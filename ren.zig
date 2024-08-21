extern fn init_window(width: c_int, height: c_int, title: [*c]u8) void;

/// creates a window if the target is native otherwise
/// it creates the index.html and index.js file for the
/// application the width and height are currently
/// ignored since the canvas will be fullscreen anyways
/// and the browser can then be resized (can be changed if many people want that)
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
