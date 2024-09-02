const std = @import("std");
const glfw = @cImport({
    @cDefine("GLFW_INCLUDE_VULKAN", {});
    @cInclude("GLFW/glfw3.h");
});

const t_ren = extern struct {
    window: ?*glfw.GLFWwindow,

    instance: glfw.VkInstance,
    surface: glfw.VkSurfaceKHR,

    device: glfw.VkDevice,
    physical_device: glfw.VkPhysicalDevice,

    graphics_queue: glfw.VkQueue,
    present_queue: glfw.VkQueue,

    swap_chain: glfw.VkSwapchainKHR,
    swap_chain_extent: glfw.VkExtent2D,
    swap_chain_images: ?*glfw.VkImage,
    swap_chain_image_views: ?*glfw.VkImageView,
    swap_chain_images_size: usize,
    swap_chain_framebuffers: ?*glfw.VkFramebuffer,
    swap_chain_image_format: glfw.VkFormat,

    render_pass: glfw.VkRenderPass,
    pipeline_layout: glfw.VkPipelineLayout,
    graphics_pipeline: glfw.VkPipeline,

    command_pool: glfw.VkCommandPool,
    command_buffers: ?*glfw.VkCommandBuffer,

    vertex_buffer: glfw.VkBuffer,
    vertex_buffer_memory: glfw.VkDeviceMemory,

    in_flight_fences: ?*glfw.VkFence,
    image_available_semaphores: ?*glfw.VkSemaphore,
    render_finished_semaphores: ?*glfw.VkSemaphore,

    current_frame: c_uint,
};

extern fn ren_init(width: c_int, height: c_int, title: [*c]u8) t_ren;
extern fn ren_destroy(ren: *t_ren) void;

extern fn ren_draw_frame(ren: *t_ren) void;

pub const Ren = struct {
    ren: t_ren,

    const Self = @This();

    pub fn init(width: i32, height: i32, title: []const u8) Self {
        return .{
            .ren = ren_init(
                @intCast(width),
                @intCast(height),
                @as([*c]u8, @constCast(@alignCast(title))),
            ),
        };
    }

    pub fn drawFrame(self: *Self) void {
        ren_draw_frame(&self.ren);
    }

    pub fn deinit(self: *Self) void {
        ren_destroy(@as(*t_ren, @ptrCast(@alignCast(self))));
    }
};

test "can access extern fn" {
    var ren = Ren.init(100, 100, "example");
    ren.deinit();
}
