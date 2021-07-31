#include <math.h>
#include <pluto.h>
#include <signal.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

int haw, hah;
int mup_w, mdo_w;
const int warp = 5;
bool rdrw = false;
struct termios t_chan, t_orig;

float transform(float x) // Return Y for X
{
	return x * sinf(x) * cosf(x);
}

void enable_raw_mode()
{
	tcgetattr(STDIN_FILENO, &t_orig);
	t_chan = t_orig;
	t_chan.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &t_chan);
}

void cleanup()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &t_orig);
	pluto_deinit();
}

void plot()
{
	pluto_clear_buffers();

	for (int x = mdo_w; x <= mup_w; x++)
	{
		int y = (int)transform(x);

		pluto_set_pix(haw + x, hah - y);
	}
		
	pluto_write_out();
	pluto_render();
	
	rdrw = false;
}

void step()
{
	bool exit = false;
	char key = 0;

	while (!exit)
	{
		read(STDIN_FILENO, &key, 1);

		switch (key)
		{
			case 'w':
				hah += warp;
				rdrw = true;
				break;
			case 'a':
				haw += warp;
				rdrw = true;
				break;
			case 's':
				hah -= warp;
				rdrw = true;
				break;
			case 'd':
				haw -= warp;
				rdrw = true;
				break;

			case 'e':
				exit = true;
				break;

			default:
				break;
		}

		if (rdrw) plot();
	}
}

int main()
{
	pluto_init_window();
	pluto_save_screen();
	enable_raw_mode();

	haw = _pluto_canvas.cwidth / 2;
	hah = _pluto_canvas.cheight / 2;

	mup_w = haw, mdo_w = -haw;
	signal(SIGINT, cleanup);

	step();

	cleanup();
	return 0;
}
