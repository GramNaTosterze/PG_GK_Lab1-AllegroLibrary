
#include <math.h>
#include <stdio.h>

// Dołącz definicje biblioteki Allegro
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz będzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 640;	//szerokość okna
const int SCREEN_H = 480;	//wysokość okna

void draw_star(float* points, int N, int R, ALLEGRO_COLOR color, double &fi, double &angle) {
	int x = SCREEN_W / 2, y = SCREEN_H / 2;
	double dfi = 0.1, pi = 3.14125;
	for (int i = 0; i < N; i++) {
		if (i % 2 == 0) {
			points[2 * i] = x + R * cos(2*pi/N * i + fi);
			points[2 * i + 1] = y + R * sin(2*pi/N * i + fi);
		}
		else {
			points[2 * i] = x + R/2 * cos(2 * pi / N * i + fi);
			points[2 * i + 1] = y + R/2 * sin(2 * pi / N * i + fi);
		}
	}

	al_draw_polygon(points, N,2,color, 2, 2);
	angle += 0.05;
	fi += 0.01;
}

// Funkcja główna
int main()
{
	ALLEGRO_DISPLAY *display = NULL;			//okno
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;	//kolejka zdarzen
	ALLEGRO_TIMER *timer = NULL;				//timer, od ktorego będziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							//inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}
	
	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obsługi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarzeń
	
	al_init_primitives_addon();							//inicjalizacja obsługi prostych elementów (punkty, linie, prostokąty, elipsy itd.)

	//Rejestracja żródeł zdarzeń (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	//Kolory rysowania
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);

	//Definicja wielokąta
	const int N = 10;
	float dx[N] = { 0.0, -30.0, -10.0, -10.0, 10.0, 10.0, 30.0 };
	float dy[N] = { -60.0, -30.0,-30.0, 60.0, 60.0,-30.0,-30.0 };

	//Tablice na przetworzone współrzędna punktów
	float points[2*N];

	//Zmienne na potrzeby obracania figury
	double fi=0.0, dfi=0.1, sinfi, cosfi, angle = 0.0;

	//Uruchamiamy timer, który będzie z zadaną częstotliwością wysyłał zdarzenia
	al_start_timer(timer);
	
	//Pętla główna programu - obsługa zdarzeń.
	//Działamy, dopóki użytkownik nie wciśnie Esc.
	while(true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> odświeżenie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> jeśli Esc to kończymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamknięcia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			redraw = false;
			al_clear_to_color(black); //czyszczenie okna na zadany kolor

			//Wyznacz środek ekranu
			int xm = SCREEN_W / 2;
			int ym = SCREEN_H / 2;

			//Obrót figury
			sinfi = sin(fi);
			cosfi = cos(fi);
			/*for (int i = 0; i < N; i++)
			{
				points[2 * i] = (dx[i] * cosfi - dy[i] * sinfi + 0.5) + xm;
				points[2 * i + 1] = (dx[i] * sinfi + dy[i] * cosfi + 0.5) + ym;
			}
			fi += dfi;
			*/
			//Narysuj wypełniony okrąg
			al_draw_filled_circle(xm, ym, 100, blue);
			al_draw_circle(xm, ym, 100, yellow, 2);

			//Narysuj wypełniony wielokat
			//al_draw_filled_polygon(points, N, white);
			draw_star(points,N,80,yellow,fi, angle);
			//Wyświetl w oknie to, co narysowano w buforze
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}
// Damian Strojek s184407

/*#include <math.h>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 60;		//obraz bêdzie aktualizowany co 1/FPS sekundy
const int SCREEN_W = 1024;	//szerokoœæ okna
const int SCREEN_H = 768;	//wysokoœæ okna
const double przesuniecie = 0.05;

void star(const int N, const int R, float points[], ALLEGRO_COLOR color, double& fi, double& angle) {
	double dfi = 0.1, pi = 3.14125;

	// Obracanie gwiazdy w ósemce
	int x = 150 * cos(angle) + SCREEN_W / 2;
	int y = 150 * sin(2 * angle) / 2 + SCREEN_H / 2;
	angle += przesuniecie;
	if (angle > 2 * pi) {
		angle -= 2 * pi;
	}

	// Rysowanie gwiazdy
	for (int i = 0; i < N; i++) {
		if (i % 2 == 0) {
			points[2 * i] = x + R * cos(2 * i * pi / N + fi);
			points[2 * i + 1] = y + R * sin(2 * i * pi / N + fi);
		}
		else {
			points[2 * i] = x + R / 2 * cos(2 * i * pi / N + fi);
			points[2 * i + 1] = y + R / 2 * sin(2 * i * pi / N + fi);
		}
	}
	fi += dfi;

	al_draw_polygon(points, N, 2, color, 2, 2);
};

// Funkcja g³ówna
int main()
{
	ALLEGRO_DISPLAY* display = NULL;			// okno
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;	// kolejka zdarzen
	ALLEGRO_TIMER* timer = NULL;				// timer, od ktorego bêdziemy odbierac zdarzenia (potrzebny do animacji)
	bool redraw = true;

	if (!al_init()) {							// inicjalizacja biblioteki Allegro
		fprintf(stderr, "Nie zainicjalizowano allegro!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);	//utworznie okna
	timer = al_create_timer(1.0 / FPS);					//utworzenie timera
	al_install_keyboard();								//inicjalizacja obs³ugi klawiatury
	event_queue = al_create_event_queue();				//utworzenie kolejki zdarzeñ

	al_init_primitives_addon();							//inicjalizacja obs³ugi prostych elementów (punkty, linie, prostok¹ty, elipsy itd.)

	//Rejestracja ¿róde³ zdarzeñ (okno, timer, klawiatura ...)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Kolory rysowania
	ALLEGRO_COLOR yellow = al_map_rgb(255, 255, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);
	ALLEGRO_COLOR blue = al_map_rgb(0, 0, 255);
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR red = al_map_rgb(255, 0, 0);

	//Definicja wielok¹ta
	const int N = 10, R = 80;
	float points[2 * N];
	double fi = 0.0, angle = 0.0;

	// Uruchamiamy timer, który bêdzie z zadan¹ czêstotliwoœci¹ wysy³a³ zdarzenia
	al_start_timer(timer);

	//Pêtla g³ówna programu - obs³uga zdarzeñ.
	//Dzia³amy, dopóki u¿ytkownik nie wciœnie Esc.
	while (true)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_TIMER) {	//zdarzenie timera -> odœwie¿enie obrazu 
			redraw = true;
		}
		else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {	//zdarzenie klawiatury -> jeœli Esc to koñczymy
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				break;
		}
		else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { //zdarzenie zamkniêcia okna
			break;
		}

		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;
			al_clear_to_color(black); //czyszczenie okna na zadany kolor
			star(N, R, points, red, fi, angle);
			al_flip_display();
		}
	}

	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	return 0;
}
*/