const std = @import("std");
const Ren = @import("ren").Ren;

pub fn main() !void {
    var ren = Ren.init(300, 300, "example");
    defer ren.deinit();
    var counter: u32 = 0;

    while (counter < 500) : (counter += 1) {
        const oa = .{ .x = 0.0, .y = -0.5 };
        const ob = .{ .x = 0.25, .y = 0 };
        const oc = .{ .x = -0.25, .y = 0 };

        const la = .{ .x = -0.25, .y = 0.0 };
        const lb = .{ .x = 0.00, .y = 0.5 };
        const lc = .{ .x = -0.50, .y = 0.5 };

        const ra = .{ .x = 0.25, .y = 0 };
        const rb = .{ .x = 0.5, .y = 0.5 };
        const rc = .{ .x = 0, .y = 0.5 };

        ren.triangle(oa, ob, oc);
        ren.triangle(la, lb, lc);
        ren.triangle(ra, rb, rc);

        ren.draw();
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
