# Arcpack-rs
Arcpack-rs is the continuation of the arcpack utility.
arcpack was buggy, lacked safety and only supported unix-like OSes
Arcpack-rs was make to fix these short-comings.

# Performance
Arcpack-c is way faster

# Compatability
Due to the difference in handling directories,
arcpack-c cannot unpack arcpack-rs's archives but arcpack-rs is
compatible with arcpack-c. For that reason, arcpack-rs use's header version 2
so they are not compatible with each other. Although compatibility with version 1 is being worked on.

# Warning
Arcpack-rs support for these features are not completed yet.
* symlink support
* file permission support
* file metadata support
* endiannese
* version 1 support
