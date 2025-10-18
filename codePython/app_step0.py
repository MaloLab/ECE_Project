import tkinter as tk

WIDTH, HEIGHT = 480, 640
FPS_MS = 16  # ~60 FPS

class App(tk.Tk):
    def __init__(self):
        
        super().__init__()
        self.title("FLAPIC-BIRD — Étape 0 (squelette états)")
        self.geometry(f"{WIDTH}x{HEIGHT}")
        self.resizable(False, False)

        # États: MENU | PLAYING | GAME_OVER
        self.state = "MENU"

        # Canvas unique
        self.canvas = tk.Canvas(self, width=WIDTH, height=HEIGHT, highlightthickness=0, bg="white")
        self.canvas.pack(fill="both", expand=True)

        # Plan B clavier (le bouton série viendra après)
        self.bind_all("<space>", self.on_space)

        # Premier rendu + boucle
        self.render_screen()
        self.after(FPS_MS, self.loop)

    # ----- Machine d'états -----
    def set_state(self, new_state: str):
        print(f"[STATE] {self.state} -> {new_state}")
        self.state = new_state
        self.render_screen()

    def on_space(self, _evt):
        if self.state == "MENU":
            self.set_state("PLAYING")
        elif self.state == "PLAYING":
            self.set_state("GAME_OVER")
        elif self.state == "GAME_OVER":
            self.set_state("MENU")

    # ----- Rendu par état -----
    def render_screen(self):
        self.canvas.delete("all")
        if self.state == "MENU":
            self.canvas.create_text(WIDTH//2, HEIGHT//3, text="FLAPIC-BIRD",
                                    font=("Arial", 28, "bold"))
            self.canvas.create_text(WIDTH//2, HEIGHT//3 + 60,
                                    text="Press SPACE to Start",
                                    font=("Arial", 18))
            self.canvas.create_text(WIDTH//2, HEIGHT - 40,
                                    text="(Étape 0 : squelette d'états)", font=("Arial", 10))
        elif self.state == "PLAYING":
            self.canvas.create_text(WIDTH//2, HEIGHT//2, text="PLAYING",
                                    font=("Arial", 28, "bold"))
            self.canvas.create_text(WIDTH//2, HEIGHT//2 + 40,
                                    text="Press SPACE → GAME OVER", font=("Arial", 14))
        elif self.state == "GAME_OVER":
            self.canvas.create_text(WIDTH//2, HEIGHT//2 - 10, text="GAME OVER",
                                    font=("Arial", 28, "bold"))
            self.canvas.create_text(WIDTH//2, HEIGHT//2 + 30,
                                    text="Press SPACE → MENU", font=("Arial", 14))

    # ----- Boucle (tick) -----
    def loop(self):
        # Étape 0 : structure uniquement
        self.after(FPS_MS, self.loop)

if __name__ == "__main__":
    App().mainloop()
