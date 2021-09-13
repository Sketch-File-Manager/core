    # Sketch core

## Instruction set

### Simple commands
Simple commands are executed immediately.

Command |  Description
---------|--------------
do mkdir `dst` `folder` `permissions` | Create a new `folder` to `dst`, if name contains space it should be inside quotes.
do mkfile `dst` `file` `permissions` | Create a new `file` at `dst` directory, if name contains space it should be inside quotes.
do copy `src` `dst` | Copy a `src` (file or directory) to `dst` folder.
do move `src` `dst` | Move a `src` (file or directory) to `dst` folder.
do rename `src` `new_name` | Rename `src` (file or directory) to `new_name`
do edit [flags] `src` `content` | Edit a `src` file and change its content. `-a` or `--append`: append to end. `-u` or `--unshift`: append to start. `-w` or `--write`: Discard old content and append new.
do permissions `src` `permissions` `recursive` | Change permissions to src. If src is folder and recursive is `1` then all sub folders and files will receive the same permissions.

### Session commands
Session commands are used to create a session of commands that will be executed when the user decides to.

Command |  Description
---------|--------------
session start | Begin new session (File creation).
session end `id` | Delete a session with a specific `id` (File delete).
session use `id` | Set a session with `id` as current (current-session file set `id`).
session run `id` | Execute a session with `id` (Read session file and execute).
session current | Show the current session id.
session show `id` | Shows the steps of a session with `id`.
session list | List all sessions.

#### Active session commands
Active session commands are all the `simple commands` without the `do` prefix and the commands below. Every time a command is requested it will be saved in a session file.

Command |  Description
---------|--------------
exit | Remove current session as current.
undo | Undo the last command.
