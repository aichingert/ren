const std = @import("std");
const Ren = @import("ren").Ren;

pub fn main() !void {
    var ren = Ren.init(300, 300, "example");
    defer ren.deinit();
    var counter: u32 = 0;
    var offset: f32 = 0.0001;

    while (counter < 500) : (counter += 1) {
        ren.triangle(
            .{ .x = -0.5 + offset, .y = -0.5 + offset },
            .{ .x = 0.5 + offset, .y = -0.5 + offset },
            .{ .x = 0.5 + offset, .y = 0.5 + offset },
        );

        offset += 0.0002;
        ren.draw();
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
