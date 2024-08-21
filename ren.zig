extern fn create_window(width: c_int, height: c_int) void;

pub fn createWindow(width: i32, height: i32) void {
    create_window(@intCast(width), @intCast(height));
}

test "can access extern fn" {
    //const std = @import("std");
    createWindow(10, 20);
    //try std.testing.expectEqual(add(10, 20), 30);
}
