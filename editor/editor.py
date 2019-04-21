#!/usr/bin/python3

import json, sys, os

import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk


class EditorWindow(Gtk.Window):

    def __init__(self, target_file = None):
        Gtk.Window.__init__(self, title="Tablut Board Editor")
        self.set_default_size(600, 680)
        self.set_resizable(False)

        self.board = [[0, 1, 1, 2, 2, 2, 1, 1, 0],
                      [1, 0, 0, 0, 2, 0, 0, 0, 1],
                      [1, 0, 0, 0, 0, 0, 0, 0, 1],
                      [2, 0, 0, 0, 0, 0, 0, 0, 2],
                      [2, 2, 0, 0, 3, 0, 0, 2, 2],
                      [2, 0, 0, 0, 0, 0, 0, 0, 2],
                      [1, 0, 0, 0, 0, 0, 0, 0, 1],
                      [1, 0, 0, 0, 2, 0, 0, 0, 1],
                      [0, 1, 1, 2, 2, 2, 1, 1, 0]]

        self.pawns = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0]]

        self.target_file = target_file
        self.board_modified = False
        self.board_mode = 1

        vbox = Gtk.VBox()

        self.toolbar = Gtk.Toolbar()
        new_btn = Gtk.ToolButton(Gtk.STOCK_NEW)
        self.toolbar.add(new_btn)
        new_btn.set_tooltip_text("New Board")
        new_btn.connect("clicked", self.on_new_board)

        open_btn = Gtk.ToolButton(Gtk.STOCK_OPEN)
        self.toolbar.add(open_btn)
        open_btn.set_tooltip_text("Open Board")
        open_btn.connect("clicked", self.on_open_request)

        save_btn = Gtk.ToolButton(Gtk.STOCK_SAVE)
        self.toolbar.add(save_btn)
        save_btn.set_tooltip_text("Save Board")
        save_btn.connect("clicked", self.on_save_request)

        self.toolbar.add(Gtk.SeparatorToolItem())

        black_img = Gtk.Image()
        black_img.set_from_file(resolve_resource("resources/blacks.png"))
        self.black_btn = Gtk.ToolButton()
        self.black_btn.set_icon_widget(black_img)
        self.toolbar.add(self.black_btn)
        self.black_btn.set_tooltip_text("Black Pawns")
        self.black_btn.connect("clicked", self.on_change_mode)

        white_img = Gtk.Image()
        white_img.set_from_file(resolve_resource("resources/whites.png"))
        self.white_btn = Gtk.ToolButton()
        self.white_btn.set_icon_widget(white_img)
        self.toolbar.add(self.white_btn)
        self.white_btn.set_tooltip_text("White Pawns")
        self.white_btn.connect("clicked", self.on_change_mode)

        king_img = Gtk.Image()
        king_img.set_from_file(resolve_resource("resources/kings.png"))
        self.king_btn = Gtk.ToolButton()
        self.king_btn.set_icon_widget(king_img)
        self.toolbar.add(self.king_btn)
        self.king_btn.set_tooltip_text("King Pawns")
        self.king_btn.connect("clicked", self.on_change_mode)

        self.delete_btn = Gtk.ToolButton(Gtk.STOCK_CLEAR)
        self.toolbar.add(self.delete_btn)
        self.delete_btn.set_tooltip_text("Delete Pawns")
        self.delete_btn.connect("clicked", self.on_change_mode)

        vbox.pack_start(self.toolbar, False, False, 0)

        frame = Gtk.Frame(label="Board", label_xalign=0.5)
        self.grid = Gtk.Grid(column_homogeneous=True, row_homogeneous=True)
        frame.add(self.grid)
        vbox.pack_start(frame, True, True, 0)

        self.statusbar = Gtk.Statusbar()
        self.statusbar.set_vexpand(False)

        hbox = Gtk.HBox()
        hbox.pack_start(self.statusbar, True, True, 0)
        self.turn_combobox = Gtk.ComboBoxText()
        self.turn_combobox.insert_text(0, "WHITE")
        self.turn_combobox.insert_text(1, "BLACK")
        self.turn_combobox.set_active(0)
        hbox.pack_start(self.turn_combobox, False, False, 0)

        vbox.pack_start(hbox, False, False, 0)

        self.add(vbox)

        self.buttons = []

        for y in range(9):
            self.buttons.append([])
            for x in range(9):
                button = Gtk.Button()
                button.set_hexpand(True)
                button.set_vexpand(True)

                button.get_style_context().add_class("board-btn")
                if self.board[y][x] == 0:
                    button.get_style_context().add_class("empty-btn")
                elif self.board[y][x] == 1:
                    button.get_style_context().add_class("winpoint-btn")
                elif self.board[y][x] == 2:
                    button.get_style_context().add_class("citadel-btn")
                elif self.board[y][x] == 3:
                    button.get_style_context().add_class("throne-btn")

                button.board_x = x
                button.board_y = y

                button.drag_source_set(Gdk.ModifierType.BUTTON1_MASK, [], Gdk.DragAction.MOVE)
                button.drag_dest_set(Gtk.DestDefaults.ALL, [], Gdk.DragAction.MOVE)
                button.drag_source_add_text_targets()
                button.drag_dest_add_text_targets()

                button.connect("clicked", self.on_board_button_clicked)
                button.connect("enter", self.on_board_button_hover)
                button.connect("drag-data-received", self.on_drag_data_received)
                button.connect("drag-data-get", self.on_drag_data_get)

                self.buttons[y].append(button)
                self.grid.attach(button, x, y, 1, 1)

            label = Gtk.Label(str(y + 1))
            label.set_hexpand(True)
            label.get_style_context().add_class("board-label")
            self.grid.attach(label, 9, y, 1, 1)

        for x in range(9):
            label = Gtk.Label(str(chr(ord("a") + x)).upper())
            label.set_vexpand(True)
            label.get_style_context().add_class("board-label")
            self.grid.attach(label, x, 9, 1, 1)

        self.render_board()

        if self.target_file is not None:
            self.load_board()

    def render_board(self):
        for y in range(9):
            for x in range(9):
                if self.pawns[y][x] == 0:
                    self.buttons[y][x].set_image(None)
                elif self.pawns[y][x] == 1:
                    image = Gtk.Image()
                    image.set_from_file(resolve_resource("resources/black.png"))
                    self.buttons[y][x].set_image(image)
                elif self.pawns[y][x] == 2:
                    image = Gtk.Image()
                    image.set_from_file(resolve_resource("resources/white.png"))
                    self.buttons[y][x].set_image(image)
                elif self.pawns[y][x] == 3:
                    image = Gtk.Image()
                    image.set_from_file(resolve_resource("resources/king.png"))
                    self.buttons[y][x].set_image(image)

        modified = ""
        name = "Untitled"
        if self.target_file is not None:
            name = self.target_file
        if self.board_modified:
            modified = "*"
        title = f"Tablut Board Editor [{modified}{name}]"
        self.set_title(title)

    def on_board_button_clicked(self, widget):
        self.pawns[widget.board_y][widget.board_x] = self.board_mode
        self.board_modified = True
        self.render_board()

    def on_board_button_hover(self, widget):
        cell = str(chr(ord("a") + widget.board_x)) + str(widget.board_y + 1)
        message = f"x: {widget.board_x} | y: {widget.board_y} | {cell}"
        self.statusbar.push(1, message)

    def on_drag_data_received(self, widget, drag_context, x, y, data, info, time):
        sx, sy = data.get_text().split(":")
        x, y = int(sx), int(sy)
        pawn = self.pawns[y][x]
        self.pawns[y][x] = 0
        self.pawns[widget.board_y][widget.board_x] = pawn
        self.board_modified = True
        self.render_board()

    def on_drag_data_get(self, widget, drag_context, data, info, time):
        data.set_text(f"{widget.board_x}:{widget.board_y}", -1)

    def on_new_board(self, widget):
        if self.board_modified:
            d = Gtk.MessageDialog(transient_for=self,
                                  modal=True,
                                  buttons=Gtk.ButtonsType.OK_CANCEL)
            d.props.text = "Board has not been saved, do you want to proceed anyway?"
            response = d.run()
            d.destroy()
            if response != Gtk.ResponseType.OK:
                return

        self.pawns = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0]]
        self.board_modified = False
        self.target_file = None
        self.render_board()

    def on_open_request(self, widget):
        dialog = Gtk.FileChooserDialog("Please choose board file", self,
                                       Gtk.FileChooserAction.OPEN,
                                       (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                                        Gtk.STOCK_OPEN, Gtk.ResponseType.OK))
        response = dialog.run()
        if response == Gtk.ResponseType.OK:
            self.target_file = dialog.get_filename()
            self.load_board()

        dialog.destroy()

    def on_save_request(self, widget):
        if self.target_file is None:
            dialog = Gtk.FileChooserDialog("Please choose destination file", self,
                                           Gtk.FileChooserAction.SAVE,
                                           (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                                            Gtk.STOCK_SAVE, Gtk.ResponseType.OK))
            if self.target_file is not None:
                dialog.set_filename(self.target_file)
            response = dialog.run()
            if response == Gtk.ResponseType.OK:
                self.target_file = dialog.get_filename()
                if ".tbbf" not in self.target_file:
                    self.target_file = self.target_file + ".tbbf"

            dialog.destroy()

        self.save_board()

    def load_board(self):
        if self.target_file is None:
            return

        with open(self.target_file, "r") as f:
            j = json.loads(f.read())
            self.pawns = [[0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0],
                          [0, 0, 0, 0, 0, 0, 0, 0, 0]]

            for y, row in enumerate(j["board"]):
                for x, cell in enumerate(row):
                    if cell == "WHITE":
                        self.pawns[y][x] = 2
                    elif cell == "BLACK":
                        self.pawns[y][x] = 1
                    elif cell == "KING":
                        self.pawns[y][x] = 3

            if j["turn"] == "WHITE":
                self.turn_combobox.set_active(0)
            else:
                self.turn_combobox.set_active(1)

        self.board_modified = False
        self.render_board()

    def save_board(self):
        if self.target_file is None:
            print("No target has been specified!")
            return

        with open(self.target_file, "w") as f:
            output = []
            for y, row in enumerate(self.pawns):
                output.append([])
                for x, cell in enumerate(row):
                    if cell == 1:
                        output[y].append("BLACK")
                    elif cell == 2:
                        output[y].append("WHITE")
                    elif cell == 3:
                        output[y].append("KING")
                    elif self.board[y][x] == 3:
                        output[y].append("THRONE")
                    else:
                        output[y].append("EMPTY")

            f.write(json.dumps({"board":output, "turn": self.turn_combobox.get_active_text()}, indent=4))
        self.board_modified = False
        self.render_board()

    def on_change_mode(self, widget):
        if widget == self.delete_btn:
            self.board_mode = 0
        elif widget == self.black_btn:
            self.board_mode = 1
        elif widget == self.white_btn:
            self.board_mode = 2
        elif widget == self.king_btn:
            self.board_mode = 3


def resolve_resource(res_path):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), res_path)


style_provider = Gtk.CssProvider()
style_provider.load_from_path(resolve_resource("resources/style.css"))
Gtk.StyleContext.add_provider_for_screen(
    Gdk.Screen.get_default(),
    style_provider,
    Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
)

target = None
if len(sys.argv) > 1:
    target = sys.argv[1]
win = EditorWindow(target)
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()
