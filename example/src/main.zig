const std = @import("std");
const r = @import("ren");
const Ren = r.Ren;

const ArrayList = std.ArrayList;
const Tuple = std.meta.Tuple(&[_]type{ [3]r.t_vec2, u8 });

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    const allocator = gpa.allocator();
    defer _ = gpa.deinit();

    var ren = Ren.init(300, 300, "example");
    defer ren.deinit();

    var list = ArrayList(Tuple).init(allocator);
    defer list.deinit();

    while (true) : (ren.draw()) {
        try list.append(.{ .{
            .{ .x = 0.0, .y = -0.5 },
            .{ .x = 0.5, .y = 0.5 },
            .{ .x = -0.5, .y = 0.5 },
        }, 2 });

        while (list.items.len > 0) {
            const t = list.pop();

            if (t[1] == 0) {
                ren.triangle(t[0][0], t[0][1], t[0][2]);
                continue;
            }

            const v = t[0];

            const ta = v[0];
            const tb = .{ .x = (v[0].x + v[1].x) / 2.0, .y = (v[0].y + v[1].y) / 2.0 };
            const tc = .{ .x = (v[0].x + v[2].x) / 2.0, .y = (v[0].y + v[2].y) / 2.0 };

            const ra = .{ .x = (v[1].x + v[0].x) / 2.0, .y = (v[1].y + v[0].y) / 2.0 };
            const rb = v[1];
            const rc = .{ .x = (v[1].x + v[2].x) / 2.0, .y = (v[1].y + v[2].y) / 2.0 };

            const la = .{ .x = (v[2].x + v[0].x) / 2.0, .y = (v[2].y + v[0].y) / 2.0 };
            const lb = .{ .x = (v[2].x + v[1].x) / 2.0, .y = (v[2].y + v[1].y) / 2.0 };
            const lc = v[2];

            try list.append(.{ .{ ta, tb, tc }, t[1] - 1 });
            try list.append(.{ .{ ra, rb, rc }, t[1] - 1 });
            try list.append(.{ .{ la, lb, lc }, t[1] - 1 });
        }
    }
}

test "simple test" {
    var list = std.ArrayList(i32).init(std.testing.allocator);
    defer list.deinit();
    try list.append(42);
    try std.testing.expectEqual(@as(i32, 42), list.pop());
}
