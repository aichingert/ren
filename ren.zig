const std = @import("std");
const glfw = @cImport({
    @cDefine("GLFW_INCLUDE_VULKAN", {});
    @cInclude("GLFW/glfw3.h");
});

const t_ren = extern struct {
    window: ?*glfw.GLFWwindow,
    instance: glfw.VkInstance,
};

extern fn init_window(width: c_int, height: c_int, title: [*c]u8) t_ren;
extern fn cleanup(ren: *t_ren) void;

pub const Ren = struct {
    window: ?*glfw.GLFWwindow,
    instance: glfw.VkInstance,

    const Self = @This();

    pub fn initWindow(width: i32, height: i32, title: []const u8) Self {
        const cpp_ren = init_window(
            @intCast(width),
            @intCast(height),
            @as([*c]u8, @constCast(@alignCast(title))),
        );

        return .{
            .window = cpp_ren.window,
            .instance = cpp_ren.instance,
        };
    }

    pub fn deinit(self: *Self) void {
        cleanup(@as(*t_ren, @ptrCast(@alignCast(self))));
    }
};

test "can access extern fn" {
    const ren = Ren.initWindow(10, 20, "example");
    defer ren.deinit();
}
