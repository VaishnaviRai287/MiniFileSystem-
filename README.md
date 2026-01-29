# Custom-C-Filesystem

A **from-scratch Unix-style filesystem** written in **C** that models how persistent storage actually works: blocks on disk, metadata laid out by hand, and explicit control over allocation, traversal, and lifetime.

This is not a wrapper around existing filesystems. It is a deliberate reconstruction.

---

## Why This Exists

Modern systems hide storage behind thick abstraction layers. This project peels those layers back.

The goal is to understand:
- how bytes become blocks,
- how blocks become files,
- and how files become a navigable hierarchy.

Every structure written to disk is intentional, inspectable, and debuggable.

---

## Disk Layout (Conceptual View)

On disk, the filesystem is divided into well-defined regions:

- **Superblock**  
  The filesystem’s identity and map. Validates the disk, defines layout, and anchors all offsets.

- **Inode Table**  
  A fixed region storing file and directory metadata.

- **Allocation Bitmaps**  
  Compact structures tracking free and used blocks.

- **Data Region**  
  Raw storage for file contents and directory entries.

This layout is loaded during mount and interpreted directly—no intermediate layers.

---

## Operational Model

Instead of exposing POSIX calls, the filesystem is driven through explicit commands that mirror internal operations.

### Lifecycle Control

- `fsformat` creates a disk from nothing  
- `fsmount` validates and activates it  
- `fsunmount` flushes metadata and exits cleanly  
- `fsshow` visualizes the internal layout  

These operations define the filesystem’s state machine.

---

### Object Management (Inodes)

Files and directories are represented by inodes that are:
- allocated explicitly,
- loaded on demand,
- written back deterministically.

Supported operations include creation, deletion, persistence, and metadata inspection.

---

### Storage Allocation

All storage is managed using **bitmap-based allocation**:
- fixed-size blocks across the entire disk,
- constant-time free block discovery,
- immediate reclamation on deletion.

Blocks are acquired only when data is written and released when no longer referenced.

---

### Reading and Writing

File I/O operates at the block level:
- reads follow inode block pointers,
- writes allocate blocks dynamically,
- no hidden buffering or implicit expansion.

You see exactly when disk space is consumed.

---

## Directories as Data

Directories are not special cases — they are files with structure.

Each directory contains a sequence of entries:
- filename
- inode number

Traversal is performed by reading directory blocks and resolving paths step by step.  
This mirrors classic Unix design and reinforces the “everything is a file” model.

---

## Building and Running

```sh
make
./diskutil fsformat disk.img
./diskutil fsmount disk.img
./diskutil fscreate file.txt
./diskutil fswrite file.txt
./diskutil fsread file.txt
./diskutil fsdestroy file.txt
```

## Design Philosophy

- Prefer clarity over cleverness  
- Prefer explicit disk writes over hidden behavior  
- Prefer faithful models over shortcuts  

This codebase is designed to be read, stepped through, and reasoned about. Abstractions are kept minimal so the relationship between on-disk data and in-memory structures remains obvious.

---

## Where This Can Go

The current design leaves clean extension points for future improvements, including:

- Multi-level block indirection (single, double, indirect pointers)  
- Crash recovery via journaling  
- Block caching and write buffering  
- File permissions and ownership metadata  
- Defragmentation utilities  
- FUSE-based mounting for native OS integration  

These features can be added without changing the fundamental disk layout.

---

## Summary

This project implements a complete Unix-style filesystem in C, starting from raw disk initialization and extending through mounting, file creation, block allocation, directory traversal, and deletion. Every component—from the superblock to inode management and bitmap-based allocation—is built explicitly to reflect how real filesystems operate at the storage level.

Rather than prioritizing performance or abstraction, the design emphasizes correctness, transparency, and control over on-disk structures. Files and directories are treated uniformly, allocation decisions are visible and deterministic, and all state transitions are managed explicitly. As a result, the codebase serves both as a working filesystem and as a readable reference for understanding operating system storage internals.
