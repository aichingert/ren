extern fn init_window(width: c_int, height: c_int, title: [*c]u8) void;

pub fn initWindow(width: i32, height: i32, title: []const u8) void {
    init_window(
        @intCast(width),
        @intCast(height),
        @as([*c]u8, @constCast(@alignCast(title))),
    );
}

test "can access extern fn" {
    initWindow(10, 20, "example");
}
