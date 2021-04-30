# diane

`diane` is a tool for quickly recording snippets of information, and recalling them later.

## Usage

`diane` is used from the command line. To record an item, invoke:

```
diane [text to capture]
```

E.g., `diane hello, world!`

Note: If the item text includes characters with special meaning to the shell (e.g., apostrophes), enclose the text in quotation marks.

To print the list of items that have been added (sorted by date/time added, ascending), invoke `diane` without arguments:

```
diane
```

`diane` looks for the environment variable `DIANE_DB` for where to access its database. If the environment variable isn't set, `diane` uses `~/.diane_db` by default.

## Building

`diane` uses `gcc` and `make` to build. Navigate to the `src` folder and invoke `make`:

```
cd src
make
```

At present, building and running `diane` requires a POSIX-compatible environment with the GNU extension functions (e.g., Linux, macOS, Windows Subsystem for Linux).

## Roadmap

The following features are planned:

- Tagging items, and recalling items by tag
- Modifying and deleting items
- Additional sort and filter options
- An interactive mode to assist with adding multiple items at a time
- Support for Windows without needing WSL or another POSIX-compatibility layer