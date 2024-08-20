const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});
    const test_step = b.step("test", "Run all the tests");

    const lib = b.addStaticLibrary(.{
        .name = "ren",
        .target = target,
        .optimize = optimize,
    });
    lib.linkLibCpp();
    lib.addIncludePath(b.path("."));

    lib.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &.{"main.cpp"},
        .flags = &.{},
    });

    const bindings = b.addModule("ren", .{
        .root_source_file = b.path("ren.zig"),
        .target = target,
        .optimize = optimize,
    });
    bindings.linkLibrary(lib);

    const test_target = b.addTest(.{
        .root_source_file = b.path("ren.zig"),
    });
    test_target.linkLibrary(lib);
    test_step.dependOn(&b.addRunArtifact(test_target).step);
}
