# Installing the Tablut Board Editor

Create the mime type

```
mkdir ~/.local/share/mime/packages -p
cp tbbf.xml ~/.local/share/mime/packages
update-mime-database ~/.local/share/mime
```

Copy the desktop entry

```
sudo cp TablutBoardEditor.desktop /usr/share/applications
```

Then you need to modify the desktop entry to point to the correct
path:

```
vim /usr/share/applications/TablutBoardEditor.desktop
```

And change the line:

```
Exec=python3 /home/freddy/projects/osarracino/editor/editor.py %f
```

To:

```
Exec=python3 <PATH_TO_THE_PROJECT>/osarracino/editor/editor.py %f
```

Then click on a `tbbf` file and select "Tablut Board Editor" as the default choice.
