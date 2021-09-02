# Sketch core

## Instruction set

#### Session creation
Command argument |  Description
---------|--------------
session start | Begin new session (File creation).
session end `id` | Delete a session with a specific `id` (File delete).
session use `id` | Set a session with `id` as current (current-session file set `id`).
session run `id` | Execute a session with `id` (Read session file and execute).
session current | Show the current session id.
session show `id` | Shows the steps of a session with `id`.
sessions list | List all sessions.

#### Active session
Command argument |  Description
---------|--------------
exit | Remove current session as current.
undo | Undo the last command.
mkdir `dst` `folder` | Create a new `folder` to `dst`, if name contains space it should be inside quotes.
mkfile `dst` `file` | Create a new `file` at `dst` directory, if name contains space it should be inside quotes.
copy `src` `dst` | Move a `src` (file or directory) to `dst` folder.
move `src` `dst` | Move a `src` (file or directory) to `dst` folder.
rename `src` `new_name` | Rename `src` (file or directory) to `new_name`
edit [flags] `src` `content` | Edit a `src` file and change its content. `-a` or `--append`: append to end. `-u` or `--unshift`: append to start. `-w` or `--write`: Discard old content and append new. 


