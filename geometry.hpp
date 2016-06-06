#include <mapbox/geometry/geometry.hpp>
#include <cstdint>

#define VT_POINT 1
#define VT_LINE 2
#define VT_POLYGON 3

#define VT_END 0
#define VT_MOVETO 1
#define VT_LINETO 2
#define VT_CLOSEPATH 7

#define VT_STRING 1
#define VT_NUMBER 2
#define VT_BOOLEAN 7

// The bitfield is to make sizeof(draw) be 16 instead of 24
// at the cost, apparently, of a 0.7% increase in running time
// for packing and unpacking.
struct draw {
	long long x : 40;
	signed char op;
	long long y : 40;
	signed char necessary;

	draw(int nop, long long nx, long long ny) {
		this->op = nop;
		this->x = nx;
		this->y = ny;
	}

	draw() {
	}
};

typedef std::vector<draw> drawvec;

drawvec decode_geometry(FILE *meta, long long *geompos, int z, unsigned tx, unsigned ty, int detail, long long *bbox, unsigned initial_x, unsigned initial_y);
void to_tile_scale(drawvec &geom, int z, int detail);
drawvec remove_noop(drawvec geom, int type, int shift);
drawvec clean_or_clip_poly(drawvec &geom, int z, int detail, int buffer, bool clip);
drawvec close_poly(drawvec &geom);
drawvec reduce_tiny_poly(drawvec &geom, int z, int detail, bool *reduced, double *accum_area);
bool point_within_tile(long long x, long long y, int z, int detail, long long buffer);
int quick_check(long long *bbox, int z, int detail, long long buffer);
drawvec simplify_lines(drawvec &geom, int z, int detail, bool mark_tile_bounds);
drawvec reorder_lines(drawvec &geom);
drawvec fix_polygon(drawvec &geom);
std::vector<drawvec> chop_polygon(std::vector<drawvec> &geoms);
void check_polygon(drawvec &geom, drawvec &before);
double get_area(drawvec &geom, size_t i, size_t j);

drawvec to_drawvec(mapbox::geometry::geometry<long long> const& g, int &type);
mapbox::geometry::geometry<long long> from_drawvec(int type, drawvec const& dv);
mapbox::geometry::geometry<long long> clip(mapbox::geometry::geometry<long long> const &g, int z, int line_detail, int buffer);
