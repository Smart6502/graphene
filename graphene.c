#include <math.h>
#include <pluto.h>
#include <termios.h>

int haw, hah;
int mup_w, mdo_w;

float transform(float x)					// Return Y for X
{
	return x * powf(cosf(x) * tanf(x), sinf(x));
}

void plot()
{
	for (int x = mdo_w; x <= mup_w; x++)			// Loop through X 
	{
		int y = (int)transform(x);			// Get Y (transform each X)

		pluto_set_pix(haw + x, hah - y);		// Offset adding
	}
		
	pluto_write_out();
	pluto_render();
}

int main()
{
	pluto_init_window();
	haw = _pluto_canvas.cwidth / 2;			// Default offset (W)[C]
	hah = _pluto_canvas.cheight / 2;		// Default offset (H)[C]

	mup_w = haw, mdo_w = -haw;			// Limit (W)

	plot();
	
	pluto_deinit();

	return 0;
}
