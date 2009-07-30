#include <stdio.h>
#include <stdlib.h>

#include "lo/lo.h"

#include "main.h"
#include "draw.h"
#include "structs.h"

// OSC Server Handlers

/* clrscr
	you can choose the color which the screen will be cleared.
	argv[0]->i --> R (0-255)
	argv[1]->i --> G (0-255)
	argv[2]->i --> B (0-255) */
int clearscreen_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
	c_clearscreen(screen, argv[0]->i, argv[1]->i, argv[2]->i);

	return 0;
}

/* draw
	argv[0]->i --> X (0-screenxres/rectx)
	argv[1]->i --> Y (0-screenyres/recty)
	argv[2]->i --> R (0-255)
	argv[3]->i --> G (0-255)
	argv[4]->i --> B (0-255) */
int draw_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
	c_drawrectangle(screen, argv[0]->i % gridx, argv[1]->i % gridy, rectx, recty, argv[2]->i, argv[3]->i, argv[4]->i);

	return 0;
}

/* chgrect
	argv[0]->i --> grid X
	argv[1]->i --> grid Y */
int chgrect_handler(const char *path, const char *types, lo_arg ** argv, int argc, void *data, void *user_data)
{
	if (argv[0]->i > 0 && argv[0]->i < screenxres) {
		rectx = argv[0]->i;
	}

	if (argv[1]->i > 1 && argv[1]->i < screenyres) {
		recty = argv[1]->i;
	}

	calcgridsize();

	return 0;
}

/* lo_server error handler */
void error_handler(int num, const char *msg, const char *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	exit(-1);
}

// Main OSC Server init function.
struct loservfd oscserver_init(char *port)
{
	struct loservfd output;

	output.s = lo_server_new(port, error_handler);
	output.fd = lo_server_get_socket_fd(output.s);

        if (!(output.fd > 0)) {
                printf("Fatal: Could not get OSC server's filedescriptor.\n");
                exit(1);
	}

	// add server handlers
	lo_server_add_method(output.s, "/clear", "iii", clearscreen_handler, NULL);
	lo_server_add_method(output.s, "/draw", "iiiii", draw_handler, NULL);
	lo_server_add_method(output.s, "/chgrect", "ii", chgrect_handler, NULL);

	return output;
}


