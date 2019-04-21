import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk


class EditorWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="Tablut Board Editor")
        self.set_default_size(600, 600)
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
                      [0, 0, 0, 0, 0, 0, 1, 0, 0],
                      [0, 0, 0, 0, 0, 0, 0, 0, 0]]

        self.header = Gtk.HeaderBar(title="Tablut Board Editor")
        self.header.set_subtitle("* Unnamed board")
        self.header.props.show_close_button = True
        self.set_titlebar(self.header)

        vbox = Gtk.VBox()

        self.grid = Gtk.Grid(column_homogeneous=True, row_homogeneous=True)
        vbox.pack_start(self.grid, True, True, 0)

        self.statusbar = Gtk.Statusbar()
        self.statusbar.set_vexpand(False)
        vbox.pack_start(self.statusbar, False, False, 0)

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

    def render_board(self):
        for y in range(9):
            for x in range(9):
                if self.pawns[y][x] == 0:
                    self.buttons[y][x].set_image(None)
                elif self.pawns[y][x] == 1:
                    image = Gtk.Image()
                    image.set_from_file("resources/black.png")
                    self.buttons[y][x].set_image(image)
                elif self.pawns[y][x] == 2:
                    image = Gtk.Image()
                    image.set_from_file("resources/white.png")
                    self.buttons[y][x].set_image(image)
                elif self.pawns[y][x] == 3:
                    image = Gtk.Image()
                    image.set_from_file("resources/king.png")
                    self.buttons[y][x].set_image(image)

    def on_board_button_clicked(self, widget):
        self.pawns[widget.board_y][widget.board_x] = 1
        self.render_board()

    def on_board_button_hover(self, widget):
        cell = str(chr(ord("a") + widget.board_x)) + str(widget.board_y + 1)
        message = f"x: {widget.board_x} | y: {widget.board_y} | Literal: {cell}"
        self.statusbar.push(1, message)

    def on_drag_data_received(self, widget, drag_context, x, y, data, info, time):
        sx, sy = data.get_text().split(":")
        x, y = int(sx), int(sy)
        pawn = self.pawns[y][x]
        self.pawns[y][x] = 0
        self.pawns[widget.board_y][widget.board_x] = pawn
        self.render_board()

    def on_drag_data_get(self, widget, drag_context, data, info, time):
        data.set_text(f"{widget.board_x}:{widget.board_y}", -1)


style_provider = Gtk.CssProvider()
style_provider.load_from_path("resources/style.css")
Gtk.StyleContext.add_provider_for_screen(
    Gdk.Screen.get_default(),
    style_provider,
    Gtk.STYLE_PROVIDER_PRIORITY_APPLICATION
)

win = EditorWindow()
win.connect("destroy", Gtk.main_quit)
win.show_all()
Gtk.main()
