#include <illa/filter.h>
#include <illa/types.h>
#include <orz/logger.h>
#include <orz/stopwatch.h>
#include <orz/types.h>
#include <boost/locale.hpp>
#include <random>

using Filter::FilterBuffer;
using namespace Geom;

enum
{
	SourceWidthNoResample = 10000,
	SourceHeightNoResample = 10000,
	SourceWidthResample = 500,
	SourceHeightResample = 250,
	NumRunsNoResample = 10,
	NumRunsResample = 4,
	Warmups = 1,
};

float DefaultZoom = 2.0f;


struct FilterDataPair
{
	FilterBuffer	fb;
	uint8_t*			data;
};

#include "orz/orz_math.h"

std::random_device rd;
std::default_random_engine r(rd());
std::uniform_int_distribution<> dist(0, 255);

FilterDataPair ConstructBuffer(int width, int height, int stride, int ps, const Img::Palette& pal)
{

	FilterDataPair fdp;
	fdp.data = new uint8_t[stride * height];
	for(int i = 0; i < stride * height; ++i) {
		uint8_t b = dist(r);
		if (b < 200) b = 0;
		fdp.data[i] = b;
	}
	fdp.fb.Construct(Geom::SizeInt(width, height), ps, fdp.data, stride, pal);
	return fdp;
}


FilterDataPair ConstructBuffer(int width, int height, int stride, int ps)
{
	Img::Palette p;
	return ConstructBuffer(width, height, stride, ps, p);
}

template< class _T>
void TestNoResample(_T T)
{
	FilterDataPair src = ConstructBuffer(SourceWidthNoResample, SourceHeightNoResample, 4 * SourceWidthNoResample, 4);
	FilterDataPair dst = ConstructBuffer(SourceWidthNoResample, SourceHeightNoResample, 4 * SourceWidthNoResample, 4);
	Util::StopWatch sw;

	RectInt region(PointInt(0, 0), SizeInt(SourceWidthNoResample, SourceHeightNoResample));

	for (int i = 0; i < Warmups; ++i)
		T.Run(src.fb, dst.fb, region);

	sw.Start();
	for (int i = 0; i < NumRunsNoResample; ++i)
		T.Run(src.fb, dst.fb, region);
	int time = sw.Stop();
	std::cout << "Time: " << (time / NumRunsNoResample) << "\n";
}


template< class _T>
void TestResample(_T T, Img::Format fmt)
{
	int ps = Img::EstimatePixelSize(fmt);
	FilterDataPair src = ConstructBuffer(SourceWidthResample, SourceHeightResample, ps * SourceWidthResample, ps);
	FilterDataPair dst = ConstructBuffer(RoundCast(SourceWidthResample * DefaultZoom), RoundCast(SourceHeightResample * DefaultZoom), 2 * 4 * SourceWidthResample, 4);
	Util::StopWatch sw;

	RectInt region(
		PointInt(0, 0),
		SizeInt(RoundCast(SourceWidthResample * DefaultZoom), RoundCast(SourceHeightResample * DefaultZoom)));

	for (int i = 0; i < Warmups; ++i)
		T.Run(src.fb, dst.fb, region, fmt, DefaultZoom);

	sw.Start();
	for (int i = 0; i < NumRunsResample; ++i)
		T.Run(src.fb, dst.fb, region, fmt, DefaultZoom);
	int time = sw.Stop();
	std::cout << "Time: " << (time / NumRunsResample) << "\n";
}

struct TestNearestNeighbor {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region, Img::Format fmt, float zoom) {
		Filter::Scale::AutoFilter(Filter::Mode::NearestNeighbor, src, dst, region, fmt, zoom);
	}
};

struct TestBilinear {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region, Img::Format fmt, float zoom) {
		Filter::Scale::AutoFilter(Filter::Mode::Bilinear, src, dst, region, fmt, zoom);
	}
};

struct TestLanczos3 {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region, Img::Format fmt, float zoom) {
		Filter::Scale::AutoFilter(Filter::Mode::Lanczos3, src, dst, region, fmt, zoom);
	}
};

struct TestSolidColor {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region) {
		Filter::Alpha::SolidColor(src, dst, region, Geom::PointInt(0, 0), Img::Color(0x33, 0x22, 0x11, 0x99));
	}
};

struct TestGamma {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region) {
		Filter::Adjust::Gamma(src, dst, region, Geom::PointInt(0, 0), 30);
	}
};

struct TestBrightnessConstrast {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region) {
		Filter::Adjust::BrightnessContrast(src, dst, region, Geom::PointInt(0, 0), 30, 30);
	}
};


struct TestBlendSolid {
	void Run(FilterBuffer& src, FilterBuffer& dst, const Geom::RectInt& region) {
		Filter::Alpha::SolidColor(src, dst, region, Geom::PointInt(0, 0), Img::Color(10, 20, 30, 40));
	}
};

int realMain(std::string testToRun, std::string format)
{
	std::locale::global(boost::locale::generator().generate(""));

	testToRun = boost::locale::to_lower(testToRun);
	format = boost::locale::to_lower(format);

	Img::Format fmt = Img::Format::Undefined;
	if (format == "xrgb8888")		fmt = Img::Format::XRGB8888;
	else if (format == "argb8888")	fmt = Img::Format::ARGB8888;
	else if (format == "xrgb1555")	fmt = Img::Format::XRGB1555;
	else if (format == "argb1555")	fmt = Img::Format::ARGB1555;

	if (testToRun == "blend")
		TestNoResample(TestSolidColor());
	else if (testToRun == "gamma")
		TestNoResample(TestGamma());
	else if (testToRun == "brightness_contrast")
		TestNoResample(TestBrightnessConstrast());
	else if (testToRun == "blendsolid")
		TestNoResample(TestBlendSolid());
	else if (testToRun == "lanczos3")
		TestResample(TestLanczos3(), fmt);
	else if (testToRun == "bilinear")
		TestResample(TestBilinear(), fmt);
	else if (testToRun == "nearest")
		TestResample(TestNearestNeighbor(), fmt);
	else
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}


#ifdef _WIN32
int wmain(int argc, wchar_t* argv[]) {
	if(argc < 3) return EXIT_FAILURE;
	return realMain(WStringToUTF8(argv[1]), WStringToUTF8(argv[2]));
}
#else
int main(int argc, char* argv[]) {
	if(argc < 3) return EXIT_FAILURE;
	return realMain(argv[1], argv[2]);
}
#endif
