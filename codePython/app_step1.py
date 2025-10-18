import tkinter as tk

# -------------------- Constantes --------------------
WIDTH, HEIGHT = 960, 540
FPS_MS  = 16
BLINK_MS = 500  # 1 Hz (500 ms ON / 500 ms OFF)

MODES = ["Button", "Potentiometer", "Infrared", "Ultrasound"]
DEFAULT_MODE = "Button"

# -------------------- App --------------------
class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("FLAPIC-BIRD — Menu & States")

        # Plein écran au démarrage (F11 pour basculer)
        try:
            self.attributes("-fullscreen", True)
        except Exception:
            self.state("zoomed")
        self.resizable(True, True)

        # --- États & sélection ---
        self.state_name    = "MENU"                     # MENU | PLAYING | GAME_OVER
        self.selected_idx  = MODES.index(DEFAULT_MODE)  # index du mode sélectionné
        self.selected_mode = DEFAULT_MODE               # nom du mode sélectionné
        self.blink_on      = True                       # pour le clignotement

        # --- Canvas principal ---
        self.canvas = tk.Canvas(self, bg="white", highlightthickness=0)
        self.canvas.pack(fill="both", expand=True)
        self.canvas.bind("<Configure>", lambda e: self.render_screen())  # re-render si redimension

        # --- Bind clavier (simulation EasyPIC) ---
        self.bind_all("<Key-x>", lambda e: self._do_start())                   # Start
        self.bind_all("<Key-c>", lambda e: self._set_mode("Button"))           # Choix direct
        self.bind_all("<Key-v>", lambda e: self._set_mode("Potentiometer"))
        self.bind_all("<Key-b>", lambda e: self._set_mode("Infrared"))
        self.bind_all("<Key-n>", lambda e: self._set_mode("Ultrasound"))
        self.bind_all("<Key-a>", lambda e: self._sim_collision())              # Collision (test)
        self.bind_all("<Return>", lambda e: self._back_to_menu())              # Retour MENU depuis GAME_OVER
        self.bind_all("<Key-m>", lambda e: self._do_exit())                    # Quitter
        self.bind_all("<F11>",    self._toggle_fullscreen)                     # Plein écran

        # --- Lancements des boucles ---
        self.render_screen()
        self.after(FPS_MS, self.loop)      # tick régulier (pour le futur gameplay/série)
        self.after(BLINK_MS, self._blink)  # clignotement 1 Hz

    # ========== Machine d'états ==========
    def _set_state(self, new_state: str):
        if new_state == self.state_name:
            return
        self.state_name = new_state
        self.render_screen()  # redessine la "page"

    # ========== Sélection (menu) ==========
    def _set_mode(self, mode_name: str):
        if self.state_name != "MENU":
            return
        if mode_name in MODES:
            self.selected_mode = mode_name
            self.selected_idx  = MODES.index(mode_name)
            self.render_menu()  # met à jour l'encadrement

    # ========== Actions ==========
    def _do_start(self):
        if self.state_name == "MENU":
            self._set_state("PLAYING")

    def _sim_collision(self):
        if self.state_name == "PLAYING":
            self._set_state("GAME_OVER")

    def _back_to_menu(self):
        if self.state_name == "GAME_OVER":
            self._set_state("MENU")

    def _do_exit(self):
        self.destroy()

    def _toggle_fullscreen(self, _e=None):
        try:
            fs = self.attributes("-fullscreen")
            self.attributes("-fullscreen", not fs)
        except Exception:
            pass

    # ========== Rendu ==========
    def render_screen(self):
        self.canvas.delete("all")
        w = self.canvas.winfo_width()  or WIDTH
        h = self.canvas.winfo_height() or HEIGHT

        if self.state_name == "MENU":
            self._draw_title(w, h)
            self.render_menu()
            self._draw_footer(w, h)

        elif self.state_name == "PLAYING":
            self.canvas.create_text(w//2, h//2 - 20,
                                    text=f"PLAYING — Mode: {self.selected_mode}",
                                    font=("Arial", 24, "bold"))
            self.canvas.create_text(w//2, h//2 + 20,
                                    text="(A = collision • M = exit • F11 = fullscreen)",
                                    font=("Arial", 14))

        elif self.state_name == "GAME_OVER":
            self.canvas.create_text(w//2, h//2 - 10, text="GAME OVER",
                                    font=("Arial", 32, "bold"))
            self.canvas.create_text(w//2, h//2 + 30, text="Press ENTER → MENU",
                                    font=("Arial", 14))

    def _draw_title(self, w, h):
        self.canvas.create_text(w//2, int(h*0.18),
                                text="FLAPIC-BIRD",
                                font=("Arial", 36, "bold"))
        # Texte clignotant (tag=press_start)
        self.press_tag = "press_start"
        self.canvas.create_text(w//2, int(h*0.28),
                                text="Press X to start",
                                font=("Arial", 18),
                                tags=self.press_tag)

    def _draw_footer(self, w, h):
        self.canvas.create_text(w//2, h - 60,
            text="X=Start • C=Button • V=Pot • B=IR • N=Ultrasound • A=Crash • Enter=Menu • M=Exit",
            font=("Arial", 12))
        self.canvas.create_text(w//2, h - 35,
            text="F11: plein écran",
            font=("Arial", 11))

    def render_menu(self):
        w = self.canvas.winfo_width()  or WIDTH
        h = self.canvas.winfo_height() or HEIGHT

        top_y = int(h*0.38)
        gap_y = 56
        pad_x = 18
        pad_y = 10

        # Nettoyage des anciens items
        for i in range(len(MODES)):
            self.canvas.delete(f"menu_item_{i}")
            self.canvas.delete(f"menu_box_{i}")

        # Dessin des items + encadrement du sélectionné
        for i, name in enumerate(MODES):
            y   = top_y + i*gap_y
            tag = f"menu_item_{i}"
            style = ("Arial", 20, "bold") if i == self.selected_idx else ("Arial", 20, "normal")
            text_id = self.canvas.create_text(w//2, y, text=name, font=style, tags=tag)

            if i == self.selected_idx:
                x1, y1, x2, y2 = self.canvas.bbox(text_id)
                x1 -= pad_x; x2 += pad_x; y1 -= pad_y; y2 += pad_y
                self.canvas.create_rectangle(x1, y1, x2, y2,
                                             outline="black", width=3,
                                             tags=f"menu_box_{i}")

        # Reset du blink pour l'élément et le "Press X to start"
        self.blink_on = True
        self._set_tag_visible("press_start", True)

    # ========== Utilitaires UI ==========
    def _set_tag_visible(self, tag, visible: bool):
        for item in self.canvas.find_withtag(tag):
            self.canvas.itemconfigure(item, state=("normal" if visible else "hidden"))

    # ========== Blink 1 Hz ==========
    def _blink(self):
        if self.state_name == "MENU":
            self.blink_on = not self.blink_on
            # seul "Press X to start" clignote
            self._set_tag_visible("press_start", self.blink_on)
        self.after(BLINK_MS, self._blink)

    # ========== Boucle principale ==========
    def loop(self):
        # plus tard: lecture série / updates de gameplay
        self.after(FPS_MS, self.loop)

# -------------------- Main --------------------
if __name__ == "__main__":
    App().mainloop()
