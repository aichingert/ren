const std = @import("std");
const Ren = @import("ren").Ren;

pub fn main() !void {
    var ren = Ren.init(300, 300, "example");
    defer ren.deinit();
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
