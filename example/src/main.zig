const std = @import("std");
const ren = @import("ren");

pub fn main() !void {
    ren.createWindow(1920, 1080);
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
