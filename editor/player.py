#!/usr/bin/python3

import json, sys, os, socket, struct
from threading import Thread

import gi

gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk, GLib


class EditorWindow(Gtk.Window):

    def __init__(self, target_file = None):
        Gtk.Window.__init__(self, title="Tablut Player")
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

        vbox = Gtk.VBox()

        hbox = Gtk.HBox()
        self.host_field = Gtk.Entry(text="localhost")
        hbox.pack_start(self.host_field, True, True, 0)
        self.port_field = Gtk.Entry(text="5800")
        hbox.pack_start(self.port_field, False, False, 0)
        self.turn_combobox = Gtk.ComboBoxText()
        self.turn_combobox.insert_text(0, "WHITE")
        self.turn_combobox.insert_text(1, "BLACK")
        self.turn_combobox.set_active(0)
        self.turn_combobox.connect("changed", self.on_port_changed)
        hbox.pack_start(self.turn_combobox, False, False, 0)
        self.connect_btn = Gtk.Button("Connect")
        self.connect_btn.connect("clicked", self.on_connect_btn_clicked)
        hbox.pack_start(self.connect_btn, False, False, 0)
        vbox.pack_start(hbox, False, False, 0)

        frame = Gtk.Frame(label="Board", label_xalign=0.5)
        self.grid = Gtk.Grid(column_homogeneous=True, row_homogeneous=True)
        frame.add(self.grid)
        vbox.pack_start(frame, True, True, 0)

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
        f = str(chr(ord("a") + x)) + str(y + 1)
        to = str(chr(ord("a") + widget.board_x)) + str(widget.board_y + 1)
        turn = self.turn_combobox.get_active_text()
        self.send_move(f, to, turn)

    def on_connect_btn_clicked(self, widget):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host_field.get_text(), int(self.port_field.get_text())))
        self.net_thread = Thread(target = self.network_worker)
        self.net_thread.start()

    def network_worker(self):
        self.write_string("cj")
        while True:
            j = self.read_string()
            print(j)
            GLib.idle_add(self.load_board, j)


    def write_string(self, string):
        b = bytes(string, 'utf-8')
        self.sock.send(struct.pack('>i', len(b)))
        self.sock.send(b)

    def recvall(self, sock, n):
        # Helper function to recv n bytes or return None if EOF is hit
        data = b''
        while len(data) < n:
            packet = sock.recv(n - len(data))
            if not packet:
                return None
            data += packet
        return data

    def read_string(self):
        n = struct.unpack('>i', self.recvall(self.sock, 4))[0]
        return self.recvall(self.sock, n).decode("utf-8")

    def on_port_changed(self, widget):
        if widget.get_active() == 0:
            self.port_field.set_text("5800")
        else:
            self.port_field.set_text("5801")

    def send_move(self, f, to, turn):
        self.write_string(json.dumps({"from": f, "to": to, "turn": turn}))

    def on_drag_data_get(self, widget, drag_context, data, info, time):
        data.set_text(f"{widget.board_x}:{widget.board_y}", -1)

    def load_board(self, j):
        j = json.loads(j)
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

        self.render_board()


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
