reminder
========

A small application for usage in console that acts as a reminder for important tasks.

REQUIREMENTS
==============

sqlite3 library

BUILD
========

simply invoke:
```
$ make
```
<br>
and that should make the executable. You can then invoke it by:
```
$ ./reminder [DB LOCAITON] OPTION
```

USAGE
========

reminder [DB LOCATION] OPTION

OPTION:
-c or --create for creating<br>
-e or --edit to edit a task<br>
-d or --delete to delete a task<br>
-l or --list to list all tasks<br>
-L or --listd to list by date<br>
-t or --today to list all tasks that start/end today<br>


