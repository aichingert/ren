const std = @import("std");
const Ren = @import("ren").Ren;

pub fn main() !void {
    var ren = Ren.init(300, 300, "example");
    defer ren.deinit();
    var counter: u32 = 0;

    while (counter < 50000) : (counter += 1) {
        ren.draw();
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
