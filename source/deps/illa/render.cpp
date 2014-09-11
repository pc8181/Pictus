#include "StdAfx.h"
#include "render.h"
#include "surface.h"
#include "filter.h"

namespace Geom {
	template <typename T>
	Rect<T> RotateRect(const Rect<T>& rectToRotate, const Size<T>& boundaries, Filter::RotationAngle angle) {
		switch(angle) {
		case Filter::RotateDefault:
			return rectToRotate;

		case Filter::FlipX:
			return Geom::RectInt(
				Geom::PointInt(boundaries.Width - rectToRotate.Left() - rectToRotate.Width(), rectToRotate.Top()),
				rectToRotate.Dimensions());

		case Filter::FlipY:
			return Geom::RectInt(
				Geom::PointInt(rectToRotate.Left(), boundaries.Height - rectToRotate.Top() - rectToRotate.Height()),
				rectToRotate.Dimensions());

		case Filter::Rotate90:
			return Geom::RectInt(Geom::PointInt(rectToRotate.Top(), boundaries.Width - rectToRotate.Right()), Geom::SizeInt(rectToRotate.Height(), rectToRotate.Width()));

		case Filter::Rotate180:
			return RotateRect(RotateRect(rectToRotate, boundaries, Filter::FlipY), boundaries, Filter::FlipX);

		case Filter::Rotate270:
			return RotateRect(RotateRect(RotateRect(rectToRotate, boundaries, Filter::FlipY), boundaries, Filter::FlipX), boundaries, Filter::Rotate90);

		default:
			DO_THROW(Err::Unsupported, TX("Rotation method not yet implemented"));
		}
	}
}

namespace Img {
	void RenderToBuffer(Filter::FilterBuffer& dest, const Filter::FilterBuffer& source, Format fmt, const Geom::RectInt& region, const Properties& props) {
		using namespace Geom;

		// Apply rotation to coordinates
		Geom::SizeInt rotatedSize = RoundCast((props.Angle == Filter::Rotate90 || props.Angle == Filter::Rotate270)?
			source.Dimensions.Flipped():
			source.Dimensions);

		rotatedSize = RoundCast(rotatedSize * props.Zoom);

		RectInt dstRect = RotateRect(region, rotatedSize, props.Angle);
		if ((dstRect.Top() < 0) || (dstRect.Left() < 0)) {
			DO_THROW(Err::CriticalError, TX("Rotated region is invalid (top-left off image)."));
		}

		bool isInplace;
		Geom::SizeInt queriedDestinationSize;
		Filter::Transformation::QuerySizeRotateFixed(dest, props.Angle, &queriedDestinationSize, &isInplace);

		std::unique_ptr<uint8_t[]> tempBuf;

		Filter::FilterBuffer intermediate(dest);

		if(!isInplace) {
			int toAllocate = Area(dstRect.Dimensions()) * 4;
			tempBuf.reset(new uint8_t[toAllocate]);

			intermediate.BufferData = tempBuf.get();
			intermediate.Stride = dstRect.Width() * 4;
			intermediate.Dimensions = dstRect.Dimensions();
		}

		Filter::Scale::AutoFilter(props.ResampleFilter, source, intermediate, dstRect, fmt, props.Zoom);

		RectInt fullImageArea(PointInt(0, 0), intermediate.Dimensions);

		Filter::Adjust::Gamma(intermediate, intermediate, fullImageArea, PointInt(0,0), props.Gamma);
		Filter::Adjust::BrightnessContrast(intermediate, intermediate, fullImageArea, PointInt(0,0), props.Brightness, props.Contrast);

		if (!props.RetainAlpha && HasAlpha(fmt)) {
			Filter::Alpha::SolidColor(intermediate, intermediate, fullImageArea, PointInt(0, 0), props.BackgroundColor);
		}


		if (isInplace) {
			intermediate.Dimensions = dstRect.Dimensions();
			Filter::Transformation::RotateFixed(intermediate, intermediate, RectInt(PointInt(0, 0), intermediate.Dimensions), PointInt(0, 0), props.Angle);
		}
		else {
			//std::swap(source, intermediate);
			//intermediate.Construct(dstRect.Dimensions(), 4, dest, dest.Stride, 0);
			Filter::Transformation::RotateFixed(intermediate, dest, RectInt(PointInt(0, 0), intermediate.Dimensions), PointInt(0, 0), props.Angle);
		}
	}
}