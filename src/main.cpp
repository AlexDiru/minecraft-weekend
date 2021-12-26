#include "gfx/window.h"
#include "gfx/gfx.h"

#include "state.h"
#include "block/block.h"

// TODO: remove these
#include "world/light.h"

// global state
struct State state;

void init() {

    ECSComponentManager cm;

    printf("Init\n");
    block_init();
    state.window = &window;
    renderer_init(&state.renderer);


    state.world.componentManager = &cm;
    world_init(&state.world);

    ui_init(&state.ui);
    mouse_set_grabbed(true);

    printf("ECS NEW");
    Entity player = ecs_new(&state.world.ecs);

    printf("ECS NEW DONE");
    PositionComponent positionComponent;
    cm.add(&player, C_POSITION, &positionComponent);
    ControlComponent controlComponent;
    cm.add(&player, C_CONTROL, (void*)&controlComponent);

    PhysicsComponent physicsComponent;
    vec3s aa = {0.0f, 0.0f, 0.0f};
    vec3s bb = { 0.2f, 1.6f, 0.2f };
    physicsComponent.size[0] = aa;
    physicsComponent.size[1] = bb;
    physicsComponent.flags.gravity = true;
    physicsComponent.flags.collide = true;

    cm.add(&player, C_PHYSICS, (void*)&physicsComponent);

    MovementComponent movementComponent;
    movementComponent.speed = 1.4;
    movementComponent.sprint_speed = 2.1f;
    movementComponent.jump_height = 1.0f;

    cm.add(&player, C_MOVEMENT, (void*)&movementComponent);

    CameraComponent cameraComponent;
    cameraComponent.offset = { 0.0f, 0.8f, 0.0f };

    cm.add(&player, C_CAMERA, (void*)&cameraComponent);

    BlockLookComponent blockLookComponent;
    blockLookComponent.radius = 5.0f;
    blockLookComponent.flags.render = true;

    cm.add(&player, C_BLOCKLOOK, (void*)&blockLookComponent);
    cm.add(&player, C_LIGHT);

    PositionComponent *c_position = cm.getPositionComponent(&player);
    c_position->position = { 0, 80, 0 };

    ControlComponent *c_control = cm.getControlComponent(&player);
    c_control->mouse_sensitivity = 3.0f;

    state.world.entity_load = &player;
    state.world.entity_view = &player;
}

void destroy() {
    return;
    renderer_destroy(&state.renderer);
    world_destroy(&state.world);
    ui_destroy(&state.ui);
}

void tick() {
    return;
    state.ticks++;
    world_tick(&state.world);
    ui_tick(&state.ui);

    // time warp
    if (state.window->keyboard.keys[GLFW_KEY_L].down) {
        state.world.ticks += 30;
    }

    if (state.window->keyboard.keys[GLFW_KEY_P].pressed_tick) {
        state.world.ticks += (TOTAL_DAY_TICKS) / 3;
    }
}

void update() {
    return;
    renderer_update(&state.renderer);
    world_update(&state.world);
    ui_update(&state.ui);

    // wireframe toggle (T)
    if (state.window->keyboard.keys[GLFW_KEY_T].pressed) {
        state.renderer.flags.wireframe = !state.renderer.flags.wireframe;
    }

    // mouse toggle (ESC)
    if (state.window->keyboard.keys[GLFW_KEY_ESCAPE].pressed) {
        mouse_set_grabbed(!mouse_get_grabbed());
    }
}

void render() {
    return;
    renderer_prepare(&state.renderer, PASS_3D);
    world_render(&state.world);

    renderer_prepare(&state.renderer, PASS_2D);
    renderer_push_camera(&state.renderer);
    {
        renderer_set_camera(&state.renderer, CAMERA_ORTHO);
        ui_render(&state.ui);
    }
    renderer_pop_camera(&state.renderer);
}

int main(int argc, char *argv[]) {
    window_create(init, destroy, tick, update, render);
    window_loop();
}