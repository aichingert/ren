const std = @import("std");
const glfw = @cImport({
    @cDefine("GLFW_INCLUDE_VULKAN", {});
    @cInclude("GLFW/glfw3.h");
});

extern const frame_overlap: usize;
const todo: usize = 2; // TODO: get frome frame_overlap

const t_frame_data = extern struct {
    render_fence: glfw.VkFence,
    present_sema: glfw.VkSemaphore,
    render_sema: glfw.VkSemaphore,

    command_pool: glfw.VkCommandPool,
    command_buffer: glfw.VkCommandBuffer,
};

const t_swapchain = extern struct {
    self: glfw.VkSwapchainKHR,
    extent: glfw.VkExtent2D,
    format: glfw.VkFormat,

    images: ?*glfw.VkImage,
    image_views: ?*glfw.VkImageView,
    framebuffers: ?*glfw.VkFramebuffer,

    size: usize,
};

const t_rulkan = extern struct {
    instance: glfw.VkInstance,
    surface: glfw.VkSurfaceKHR,

    device: glfw.VkDevice,
    physical_device: glfw.VkPhysicalDevice,

    present_queue: glfw.VkQueue,
    graphics_queue: glfw.VkQueue,

    swapchain: t_swapchain,
    render_pass: glfw.VkRenderPass,

    graphics_pipeline: glfw.VkPipeline,
    pipeline_layout: glfw.VkPipelineLayout,

    frames: [todo]t_frame_data,
};

const t_ren = extern struct {
    window: ?*glfw.GLFWwindow,
    rulkan: t_rulkan,
};

extern fn ren_init(width: c_uint, height: c_uint, title: [*c]u8) t_ren;
extern fn ren_destroy(ren: *t_ren) void;

pub const Ren = struct {
    ren: t_ren,

    const Self = @This();

    pub fn init(width: u32, height: u32, title: []const u8) Self {
        return .{
            .ren = ren_init(
                @intCast(width),
                @intCast(height),
                @as([*c]u8, @constCast(@alignCast(title))),
            ),
        };
    }

    pub fn deinit(self: *Self) void {
        ren_destroy(@as(*t_ren, @ptrCast(@alignCast(self))));
    }
};

test "can access extern fn" {
    var ren = Ren.init(100, 100, "example");
    ren.deinit();
}
