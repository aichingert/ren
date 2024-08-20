pub extern fn add(a: c_int, b: c_int) c_int;

test "can access extern fn" {
    const std = @import("std");
    try std.testing.expectEqual(add(10, 20), 30);
}
