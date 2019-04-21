# Installing the Tablut Board Editor

Create the mime type

```
sudo cp tbbf.xml /usr/share/mime
sudo update-mime-database /usr/share/mime
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
