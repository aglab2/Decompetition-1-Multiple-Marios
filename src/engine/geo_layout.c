#include <ultra64.h>
#include "sm64.h"

#include "geo_layout.h"
#include "math_util.h"
#include "game/memory.h"
#include "graph_node.h"
#include "game/debug.h"

typedef void (*GeoLayoutCommandProc)(void);

GeoLayoutCommandProc GeoLayoutJumpTable[] = {
    /*GEO_CMD_BRANCH_AND_LINK           */ geo_layout_cmd_branch_and_link,
    /*GEO_CMD_END                       */ geo_layout_cmd_end,
    /*GEO_CMD_BRANCH                    */ geo_layout_cmd_branch,
    /*GEO_CMD_RETURN                    */ geo_layout_cmd_return,
    /*GEO_CMD_OPEN_NODE                 */ geo_layout_cmd_open_node,
    /*GEO_CMD_CLOSE_NODE                */ geo_layout_cmd_close_node,
    /*GEO_CMD_ASSIGN_AS_VIEW            */ geo_layout_cmd_assign_as_view,
    /*GEO_CMD_UPDATE_NODE_FLAGS         */ geo_layout_cmd_update_node_flags,
    /*GEO_CMD_NODE_ROOT                 */ geo_layout_cmd_node_root,
    /*GEO_CMD_NODE_ORTHO_PROJECTION     */ geo_layout_cmd_node_ortho_projection,
    /*GEO_CMD_NODE_PERSPECTIVE          */ geo_layout_cmd_node_perspective,
    /*GEO_CMD_NODE_START                */ geo_layout_cmd_node_start,
    /*GEO_CMD_NODE_MASTER_LIST          */ geo_layout_cmd_node_master_list,
    /*GEO_CMD_NODE_LEVEL_OF_DETAIL      */ geo_layout_cmd_node_level_of_detail,
    /*GEO_CMD_NODE_SWITCH_CASE          */ geo_layout_cmd_node_switch_case,
    /*GEO_CMD_NODE_CAMERA               */ geo_layout_cmd_node_camera,
    /*GEO_CMD_NODE_TRANSLATION_ROTATION */ geo_layout_cmd_node_translation_rotation,
    /*GEO_CMD_NODE_TRANSLATION          */ geo_layout_cmd_node_translation,
    /*GEO_CMD_NODE_ROTATION             */ geo_layout_cmd_node_rotation,
    /*GEO_CMD_NODE_ANIMATED_PART        */ geo_layout_cmd_node_animated_part,
    /*GEO_CMD_NODE_BILLBOARD            */ geo_layout_cmd_node_billboard,
    /*GEO_CMD_NODE_DISPLAY_LIST         */ geo_layout_cmd_node_display_list,
    /*GEO_CMD_NODE_SHADOW               */ geo_layout_cmd_node_shadow,
    /*GEO_CMD_NODE_OBJECT_PARENT        */ geo_layout_cmd_node_object_parent,
    /*GEO_CMD_NODE_GENERATED            */ geo_layout_cmd_node_generated,
    /*GEO_CMD_NODE_BACKGROUND           */ geo_layout_cmd_node_background,
    /*GEO_CMD_NODE_BATCHSET             */ geo_layout_cmd_node_batchset,
    /*GEO_CMD_COPY_VIEW                 */ geo_layout_cmd_copy_view,
    /*GEO_CMD_NODE_HELD_OBJ             */ geo_layout_cmd_node_held_obj,
    /*GEO_CMD_NODE_SCALE                */ geo_layout_cmd_node_scale,
    /*GEO_CMD_NOP_1E                    */ geo_layout_cmd_nop2,
    /*GEO_CMD_NOP_1F                    */ geo_layout_cmd_nop3,
    /*GEO_CMD_NODE_CULLING_RADIUS       */ geo_layout_cmd_node_culling_radius,
};

struct GraphNode gObjParentGraphNode;
struct GraphNode *gCurRootGraphNode;

/* The gGeoViews array is a mysterious one. Some background:
 *
 * If there are e.g. multiple Goombas, the multiple Goomba objects share one
 * Geo node tree describing the goomba 3D model. Since every node has a single
 * parent field and not a parent array, the parent is dynamically rebinded to
 * each goomba instance just before rendering and set to null afterwards.
 * The same happens for ObjectParentNode, which has as his sharedChild a group
 * of all 240 object nodes. Why does the ObjectParentNode exist at all, if its
 * only purpose is to temporarily bind the actual group with objects? This might
 * be another remnant to Luigi.
 *
 * When creating a root node, room for (2 + cmd+0x02) pointers is allocated in
 * gGeoViews. Except for the title screen, cmd+0x02 is 10. The 2 default ones
 * might be for Mario and Luigi, and the other 10 could be different cameras for
 * different rooms / boss fights. An area might be structured like this:
 *
 * geo_camera mode_player //Mario cam
 * geo_open_node
 *   geo_render_obj
 *   geo_assign_as_view 1   // currently unused geo command
 * geo_close_node
 *
 * geo_camera mode_player //Luigi cam
 * geo_open_node
 *   geo_render_obj
 *   geo_copy_view 1        // currently unused geo command
 *   geo_assign_as_view 2
 * geo_close_node
 *
 * geo_camera mode_boss //boss fight cam
 * geo_assign_as_view 3
 * ...
 *
 * There might also be specific geo nodes for Mario or Luigi only. Or a fixed camera
 * might not have display list nodes of parts of the level that are out of view.
 * In the end Luigi got scrapped and the multiple-camera design did not pan out,
 * so everything was reduced to a single ObjectParent with a single group, and
 * camera switching was all done in one node. End of speculation.
 */
struct GraphNode **gGeoViews;
u16 gGeoNumViews; // length of gGeoViews array

uintptr_t gGeoLayoutStack[16];
struct GraphNode *gCurGraphNodeList[32];
s16 gCurGraphNodeIndex;
s16 gGeoLayoutStackIndex; // similar to SP register in MIPS
s16 gGeoLayoutReturnIndex; // similar to RA register in MIPS
u8 *gGeoLayoutCommand;

/*
  0x00: Branch and store return address
   cmd+0x04: void *branchTarget
*/
void geo_layout_cmd_branch_and_link(void) {
    gGeoLayoutStack[gGeoLayoutStackIndex++] = (uintptr_t) (gGeoLayoutCommand + CMD_PROCESS_OFFSET(8));
    gGeoLayoutStack[gGeoLayoutStackIndex++] = (gCurGraphNodeIndex << 16) + gGeoLayoutReturnIndex;
    gGeoLayoutReturnIndex = gGeoLayoutStackIndex;
    gGeoLayoutCommand = segmented_to_virtual(cur_geo_cmd_ptr(0x04));
}

// 0x01: Terminate geo layout
void geo_layout_cmd_end(void) {
    gGeoLayoutStackIndex = gGeoLayoutReturnIndex;
    gGeoLayoutReturnIndex = gGeoLayoutStack[--gGeoLayoutStackIndex] & 0xFFFF;
    gCurGraphNodeIndex = gGeoLayoutStack[gGeoLayoutStackIndex] >> 16;
    gGeoLayoutCommand = (u8 *) gGeoLayoutStack[--gGeoLayoutStackIndex];
}

/*
  0x02: Branch
   cmd+0x04: void *branchTarget
*/
void geo_layout_cmd_branch(void) {
    if (cur_geo_cmd_u8(0x01) == 1) {
        gGeoLayoutStack[gGeoLayoutStackIndex++] = (uintptr_t) (gGeoLayoutCommand + CMD_PROCESS_OFFSET(8));
    }

    gGeoLayoutCommand = segmented_to_virtual(cur_geo_cmd_ptr(0x04));
}

// 0x03: Return from branch
void geo_layout_cmd_return(void) {
    gGeoLayoutCommand = (u8 *) gGeoLayoutStack[--gGeoLayoutStackIndex];
}

// 0x04: Open node
void geo_layout_cmd_open_node(void) {
    gCurGraphNodeList[gCurGraphNodeIndex + 1] = gCurGraphNodeList[gCurGraphNodeIndex];
    gCurGraphNodeIndex++;
    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

// 0x05: Close node
void geo_layout_cmd_close_node(void) {
    gCurGraphNodeIndex--;
    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x06: Register the current node as a view
   cmd+0x02: index

  Register the current node in the gGeoViews array at the given index
*/
void geo_layout_cmd_assign_as_view(void) {
    u16 index = cur_geo_cmd_s16(0x02);

    if (index < gGeoNumViews) {
        gGeoViews[index] = gCurGraphNodeList[gCurGraphNodeIndex];
    }

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x07: Update current scene graph node flags
   cmd+0x01: u8 operation (0 = reset, 1 = set, 2 = clear)
   cmd+0x02: s16 bits
*/
void geo_layout_cmd_update_node_flags(void) {
    u16 operation = cur_geo_cmd_u8(0x01);
    u16 flagBits = cur_geo_cmd_s16(0x02);

    switch (operation) {
        case GEO_CMD_FLAGS_RESET:
            gCurGraphNodeList[gCurGraphNodeIndex]->flags = flagBits;
            break;
        case GEO_CMD_FLAGS_SET:
            gCurGraphNodeList[gCurGraphNodeIndex]->flags |= flagBits;
            break;
        case GEO_CMD_FLAGS_CLEAR:
            gCurGraphNodeList[gCurGraphNodeIndex]->flags &= ~flagBits;
            break;
    }

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x08: Create a scene graph root node that specifies the viewport
   cmd+0x02: s16 num entries (+2) to allocate for gGeoViews
   cmd+0x04: s16 x
   cmd+0x06: s16 y
   cmd+0x08: s16 width
   cmd+0x0A: s16 height
*/
void geo_layout_cmd_node_root(void) {
    s32 i;
    struct GraphNodeRoot *graphNode;

    s16 x = cur_geo_cmd_s16(0x04);
    s16 y = cur_geo_cmd_s16(0x06);
    s16 width = cur_geo_cmd_s16(0x08);
    s16 height = cur_geo_cmd_s16(0x0A);

    // number of entries to allocate for gGeoViews array
    // at least 2 are allocated by default
    // cmd+0x02 = 0x00: Mario face, 0x0A: all other levels
    gGeoNumViews = cur_geo_cmd_s16(0x02) + 2;

    graphNode = init_graph_node_root(NULL, 0, x, y, width, height);

    // TODO: check type
    gGeoViews = main_pool_alloc(gGeoNumViews * sizeof(struct GraphNode *));

    graphNode->views = gGeoViews;
    graphNode->numViews = gGeoNumViews;

    for (i = 0; i < gGeoNumViews; i++) {
        gGeoViews[i] = NULL;
    }

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x0C << CMD_SIZE_SHIFT;
}

/*
  0x09: Create orthographic projection scene graph node
   cmd+0x02: s16 scale as a percentage (usually it's 100)
*/
void geo_layout_cmd_node_ortho_projection(void) {
    struct GraphNodeOrthoProjection *graphNode;
    f32 scale = (f32) cur_geo_cmd_s16(0x02) / 100.0f;

    graphNode = init_graph_node_ortho_projection(NULL, scale);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x0A: Create camera frustum scene graph node
   cmd+0x01: u8  if nonzero, enable frustumFunc field
   cmd+0x02: s16 field of view
   cmd+0x04: u16 near
   cmd+0x06: u16 far
   [cmd+0x08: GraphNodeFunc frustumFunc]
*/
void geo_layout_cmd_node_perspective(void) {
    struct GraphNodePerspective *graphNode;
    GraphNodeFunc frustumFunc = NULL;
    s16 fov = cur_geo_cmd_s16(0x02);
    u16 near = cur_geo_cmd_u16(0x04);
    u16 far = cur_geo_cmd_u16(0x06);

    if (cur_geo_cmd_u8(0x01) != 0) {
        // optional asm function
        frustumFunc = (GraphNodeFunc) cur_geo_cmd_ptr(0x08);
        gGeoLayoutCommand += 4 << CMD_SIZE_SHIFT;
    }

    graphNode = init_graph_node_perspective(NULL, (f32) fov, near, far, frustumFunc);

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x0B: Create a scene graph node that groups other nodes without any
  additional functionality
*/
void geo_layout_cmd_node_start(void) {
    struct GraphNodeStart *graphNode = init_graph_node_start(NULL);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

// 0x1F: No operation
void geo_layout_cmd_nop3(void) {
    gGeoLayoutCommand += 0x10 << CMD_SIZE_SHIFT;
}

/*
  0x0C: Create z-buffer-toggling scene graph node
   cmd+0x01: u8 enableZBuffer (1 = on, 0 = off)
*/
void geo_layout_cmd_node_master_list(void) {
    struct GraphNodeMasterList *graphNode = init_graph_node_master_list(NULL, cur_geo_cmd_u8(0x01));

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x0D: Create a level of detail graph node, which only renders at a certain
  distance interval from the camera.
   cmd+0x04: s16 minDistance
   cmd+0x06: s16 maxDistance
*/
void geo_layout_cmd_node_level_of_detail(void) {
    s16 minDistance = cur_geo_cmd_s16(0x04);
    s16 maxDistance = cur_geo_cmd_s16(0x06);

    struct GraphNodeLevelOfDetail *graphNode =
        init_graph_node_render_range(NULL, minDistance, maxDistance);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x0E: Create switch-case scene graph node
   cmd+0x02: s16 initialSelectedCase
   cmd+0x04: GraphNodeFunc caseSelectorFunc

  caseSelectorFunc returns an index which is used to select the child node to render.
  Used for animating coins, blinking, color selection, etc.
*/
void geo_layout_cmd_node_switch_case(void) {
    struct GraphNodeSwitchCase *graphNode =
        init_graph_node_switch_case(NULL,
                                    cur_geo_cmd_s16(0x02), // case which is initially selected
                                    0,
                                    (GraphNodeFunc) cur_geo_cmd_ptr(0x04), // case update function
                                    0);

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
 0x0F: Create a camera scene graph node (GraphNodeCamera). The focus sets the Camera's areaCen position.
  cmd+0x02: s16 camera type (changes from course to course)
  cmd+0x04: s16 posX
  cmd+0x06: s16 posY
  cmd+0x08: s16 posZ
  cmd+0x0A: s16 focusX
  cmd+0x0C: s16 focusY
  cmd+0x0E: s16 focusZ
  cmd+0x10: GraphNodeFunc func
*/
void geo_layout_cmd_node_camera(void) {
    struct GraphNodeCamera *graphNode;
    s16 *cmdPos = (s16 *) &gGeoLayoutCommand[4];

    Vec3f pos, focus;

    cmdPos = read_vec3s_to_vec3f(pos, cmdPos);
    cmdPos = read_vec3s_to_vec3f(focus, cmdPos);

    graphNode = init_graph_node_camera(NULL, pos, focus,
                                       (GraphNodeFunc) cur_geo_cmd_ptr(0x10), cur_geo_cmd_s16(0x02));

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoViews[0] = &graphNode->fnNode.node;

    gGeoLayoutCommand += 0x14 << CMD_SIZE_SHIFT;
}

/*
  0x10: Create translation & rotation scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     ((params & 0x70)>>4): fieldLayout
     (params & 0x0F): drawingLayer

   fieldLayout == 0:
    cmd+0x04: s16 xTranslation
    cmd+0x06: s16 yTranslation
    cmd+0x08: s16 zTranslation
    cmd+0x0A: s16 xRotation
    cmd+0x0C: s16 yRotation
    cmd+0x0E: s16 zRotation

   fieldLayout == 1:
    cmd+0x02: s16 xTranslation
    cmd+0x04: s16 yTranslation
    cmd+0x06: s16 zTranslation
    (rotation gets copied from gVec3sZero)

   fieldLayout == 2:
    cmd+0x02: s16 xRotation
    cmd+0x04: s16 yRotation
    cmd+0x06: s16 zRotation
    (translation gets copied from gVec3sZero)

   fieldLayout == 3:
    cmd+0x02: s16 yRotation
    (translation gets copied from gVec3sZero)
    (x and z translation are set to 0)

   [cmd+var: void *displayList]
*/
void geo_layout_cmd_node_translation_rotation(void) {
    struct GraphNodeTranslationRotation *graphNode;

    Vec3s translation, rotation;

    void *displayList = NULL;
    s16 drawingLayer = LAYER_FIRST;

    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;

    switch ((params & 0x70) >> 4) {
        case 0:
            cmdPos = read_vec3s(translation, &cmdPos[2]);
            cmdPos = read_vec3s_angle(rotation, cmdPos);
            break;
        case 1:
            cmdPos = read_vec3s(translation, &cmdPos[1]);
            vec3s_copy(rotation, gVec3sZero);
            break;
        case 2:
            cmdPos = read_vec3s_angle(rotation, &cmdPos[1]);
            vec3s_copy(translation, gVec3sZero);
            break;
        case 3:
            vec3s_copy(translation, gVec3sZero);
            vec3s_set(rotation, 0, (cmdPos[1] << 15) / 180, 0); // degrees
            cmdPos += 2 << CMD_SIZE_SHIFT;
            break;
    }

    if (params & 0x80) {
        displayList = *(void **) &cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2 << CMD_SIZE_SHIFT;
    }

    graphNode = init_graph_node_translation_rotation(NULL, drawingLayer, displayList,
                                                     translation, rotation);
    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x11: Create translation scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 xTranslation
   cmd+0x04: s16 yTranslation
   cmd+0x06: s16 zTranslation
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_translation(void) {
    struct GraphNodeTranslation *graphNode;

    Vec3s translation;

    s16 drawingLayer = LAYER_FIRST;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s(translation, &cmdPos[1]);

    if (params & 0x80) {
        displayList = *(void **) &cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2 << CMD_SIZE_SHIFT;
    }

    graphNode =
        init_graph_node_translation(NULL, drawingLayer, displayList, translation);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x12: Create ? scene graph node
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 unkX
   cmd+0x04: s16 unkY
   cmd+0x06: s16 unkZ
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_rotation(void) {
    struct GraphNodeRotation *graphNode;

    Vec3s angle;

    s16 drawingLayer = LAYER_FIRST;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s_angle(angle, &cmdPos[1]);

    if (params & 0x80) {
        displayList = *(void **) &cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2 << CMD_SIZE_SHIFT;
    }

    graphNode = init_graph_node_rotation(NULL, drawingLayer, displayList, angle);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x1D: Create scale scene graph node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x04: u32 scale (0x10000 = 1.0)
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_scale(void) {
    struct GraphNodeScale *graphNode;

    s16 drawingLayer = LAYER_FIRST;
    s16 params = cur_geo_cmd_u8(0x01);
    f32 scale = cur_geo_cmd_u32(0x04) / 65536.0f;
    void *displayList = NULL;

    if (params & 0x80) {
        displayList = cur_geo_cmd_ptr(0x08);
        drawingLayer = params & 0x0F;
        gGeoLayoutCommand += 4 << CMD_SIZE_SHIFT;
    }

    graphNode = init_graph_node_scale(NULL, drawingLayer, displayList, scale);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

// 0x1E: No operation
void geo_layout_cmd_nop2(void) {
    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x13: Create a scene graph node that is rotated by the object's animation.
   cmd+0x01: u8 drawingLayer
   cmd+0x02: s16 xTranslation
   cmd+0x04: s16 yTranslation
   cmd+0x06: s16 zTranslation
   cmd+0x08: void *displayList
*/
void geo_layout_cmd_node_animated_part(void) {
    struct GraphNodeAnimatedPart *graphNode;
    Vec3s translation;
    s32 drawingLayer = cur_geo_cmd_u8(0x01);
    void *displayList = cur_geo_cmd_ptr(0x08);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;

    read_vec3s(translation, &cmdPos[1]);

    graphNode =
        init_graph_node_animated_part(NULL, drawingLayer, displayList, translation);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x0C << CMD_SIZE_SHIFT;
}

/*
  0x14: Create billboarding node with optional display list
   cmd+0x01: u8 params
     (params & 0x80): if set, enable displayList field and drawingLayer
     (params & 0x0F): drawingLayer
   cmd+0x02: s16 xTranslation
   cmd+0x04: s16 yTranslation
   cmd+0x06: s16 zTranslation
  [cmd+0x08: void *displayList]
*/
void geo_layout_cmd_node_billboard(void) {
    struct GraphNodeBillboard *graphNode;
    Vec3s translation;
    s16 drawingLayer = LAYER_FIRST;
    s16 params = cur_geo_cmd_u8(0x01);
    s16 *cmdPos = (s16 *) gGeoLayoutCommand;
    void *displayList = NULL;

    cmdPos = read_vec3s(translation, &cmdPos[1]);

    if (params & 0x80) {
        displayList = *(void **) &cmdPos[0];
        drawingLayer = params & 0x0F;
        cmdPos += 2 << CMD_SIZE_SHIFT;
    }

    graphNode = init_graph_node_billboard(NULL, drawingLayer, displayList, translation);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand = (u8 *) cmdPos;
}

/*
  0x15: Create plain display list scene graph node
   cmd+0x01: u8 drawingLayer
   cmd+0x04: void *displayList
*/
void geo_layout_cmd_node_display_list(void) {
    struct GraphNodeDisplayList *graphNode;
    s32 drawingLayer = cur_geo_cmd_u8(0x01);
    void *displayList = cur_geo_cmd_ptr(0x04);

    graphNode = init_graph_node_display_list(NULL, drawingLayer, displayList);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

void geo_layout_cmd_node_batchset(void) {
    struct GraphNodeDisplayList *graphNode;
    s32 drawingLayer = cur_geo_cmd_u8(0x01);
    void *batchSet = cur_geo_cmd_ptr(0x04);

    graphNode = init_graph_node_batchset(NULL, drawingLayer, batchSet);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x16: Create shadow scene graph node
   cmd+0x02: s16 shadowType
   cmd+0x04: s16 shadowSolidity
   cmd+0x06: s16 shadowScale
*/
void geo_layout_cmd_node_shadow(void) {
    struct GraphNodeShadow *graphNode;
    u8 shadowType = cur_geo_cmd_s16(0x02);
    u8 shadowSolidity = cur_geo_cmd_s16(0x04);
    s16 shadowScale = cur_geo_cmd_s16(0x06);

    graphNode = init_graph_node_shadow(NULL, shadowScale, shadowSolidity, shadowType);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

// 0x17: Create scene graph node that manages the group of all object nodes
void geo_layout_cmd_node_object_parent(void) {
    struct GraphNodeObjectParent *graphNode = init_graph_node_object_parent(NULL, &gObjParentGraphNode);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x18: Create dynamically generated displaylist scene graph node
   cmd+0x02: s16 parameter
   cmd+0x04: GraphNodeFunc func
*/
void geo_layout_cmd_node_generated(void) {
    struct GraphNodeGenerated *graphNode =
        init_graph_node_generated(NULL,
                                  (GraphNodeFunc) cur_geo_cmd_ptr(0x04), // asm function
                                  cur_geo_cmd_s16(0x02));                // parameter

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x19: Create background scene graph node
   cmd+0x02: s16 background // background ID, or RGBA5551 color if backgroundFunc is null
   cmd+0x04: GraphNodeFunc backgroundFunc
*/
void geo_layout_cmd_node_background(void) {
    struct GraphNodeBackground *graphNode = init_graph_node_background(
        NULL,
        cur_geo_cmd_s16(0x02), // background ID, or RGBA5551 color if asm function is null
        (GraphNodeFunc) cur_geo_cmd_ptr(0x04), // asm function
        0);

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

// 0x1A: No operation
void geo_layout_cmd_nop(void) {
    gGeoLayoutCommand += 0x08 << CMD_SIZE_SHIFT;
}

/*
  0x1B: Copy the shared children from the object parent from a specific view
  to a newly created object parent node.
   cmd+0x02: s16 index (of gGeoViews)
*/
void geo_layout_cmd_copy_view(void) {
    struct GraphNodeObjectParent *graphNode;
    struct GraphNode *node = NULL;
    s16 index = cur_geo_cmd_s16(0x02);

    if (index >= 0) {
        node = gGeoViews[index];

        if (node->type == GRAPH_NODE_TYPE_OBJECT_PARENT) {
            node = ((struct GraphNodeObjectParent *) node)->sharedChild;
        } else {
            node = NULL;
        }
    }

    graphNode = init_graph_node_object_parent(NULL, node);

    register_scene_graph_node(&graphNode->node);

    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

/*
  0x1C: Create a held object scene graph node
   cmd+0x01: u8 unused
   cmd+0x02: s16 offsetX
   cmd+0x04: s16 offsetY
   cmd+0x06: s16 offsetZ
   cmd+0x08: GraphNodeFunc nodeFunc
*/
void geo_layout_cmd_node_held_obj(void) {
    struct GraphNodeHeldObject *graphNode;
    Vec3s offset;

    read_vec3s(offset, (s16 *) &gGeoLayoutCommand[0x02]);

    graphNode = init_graph_node_held_object(
        NULL, NULL, offset, (GraphNodeFunc) cur_geo_cmd_ptr(0x08), cur_geo_cmd_u8(0x01));

    register_scene_graph_node(&graphNode->fnNode.node);

    gGeoLayoutCommand += 0x0C << CMD_SIZE_SHIFT;
}

/*
  0x20: Create a scene graph node that specifies for an object the radius that
   is used for frustum culling.
   cmd+0x02: s16 cullingRadius
*/
void geo_layout_cmd_node_culling_radius(void) {
    struct GraphNodeCullingRadius *graphNode = init_graph_node_culling_radius(NULL, cur_geo_cmd_s16(0x02));
    register_scene_graph_node(&graphNode->node);
    gGeoLayoutCommand += 0x04 << CMD_SIZE_SHIFT;
}

struct GraphNode *process_geo_layout(void *segptr) {
    // set by register_scene_graph_node when gCurGraphNodeIndex is 0
    // and gCurRootGraphNode is NULL
    gCurRootGraphNode = NULL;

    gGeoNumViews = 0; // number of entries in gGeoViews

    gCurGraphNodeList[0] = 0;
    gCurGraphNodeIndex = 0; // incremented by cmd_open_node, decremented by cmd_close_node

    gGeoLayoutStackIndex = 2;
    gGeoLayoutReturnIndex = 2; // stack index is often copied here?

    gGeoLayoutCommand = segmented_to_virtual(segptr);

    gGeoLayoutStack[0] = 0;
    gGeoLayoutStack[1] = 0;

    while (gGeoLayoutCommand != NULL) {
        assert((gGeoLayoutCommand[0x00] < GEO_CMD_COUNT), "Invalid or unloaded geo layout detected.");
        GeoLayoutJumpTable[gGeoLayoutCommand[0x00]]();
    }

    return gCurRootGraphNode;
}
