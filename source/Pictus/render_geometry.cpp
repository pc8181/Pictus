#include "render_geometry.h"

namespace App
{
	void GenerateQuad(Geom::RectFloat position, Geom::RectFloat texCoords, Geom::SizeFloat adjustment,
					  Geom::SizeInt targetSize, Filter::RotationAngle angle, Hw3D::Vertex2D &a, Hw3D::Vertex2D &b,
					  Hw3D::Vertex2D &c, Hw3D::Vertex2D &d)
	{
		switch (angle)
		{
			case Filter::RotationAngle::RotateDefault:
				a.Position = position.TopLeft();
				a.TexCoord = texCoords.TopLeft();
				b.Position = position.TopRight();
				b.TexCoord = texCoords.TopRight();
				c.Position = position.BottomLeft();
				c.TexCoord = texCoords.BottomLeft();
				d.Position = position.BottomRight();
				d.TexCoord = texCoords.BottomRight();
				break;
			case Filter::RotationAngle::FlipX:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1), 0};
				a.Position = {targetSize.Width - position.Left() - position.Width(), position.Top()};
				a.TexCoord = texCoords.TopRight();
				b.Position = {targetSize.Width - position.Left(), position.Top()};
				b.TexCoord = texCoords.TopLeft();
				c.Position = {targetSize.Width - position.Left() - position.Width(), position.Bottom()};
				c.TexCoord = texCoords.BottomRight();
				d.Position = {targetSize.Width - position.Left(), position.Bottom()};
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::FlipY:
				adjustment += Geom::SizeFloat{0, static_cast<float>(targetSize.Height & 1)};
				a.Position = {position.Left(), targetSize.Height - position.Top() - position.Height()};
				a.TexCoord = texCoords.BottomLeft();
				b.Position = {position.Right(), targetSize.Height - position.Top() - position.Height()};
				b.TexCoord = texCoords.BottomRight();
				c.Position = {position.Left(), targetSize.Height - position.Top()};
				c.TexCoord = texCoords.TopLeft();
				d.Position = {position.Right(), targetSize.Height - position.Top()};
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate90:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1), 0};
				a.Position = {targetSize.Width - position.Top() - position.Height(), position.Left()};
				a.TexCoord = texCoords.BottomLeft();
				b.Position = {targetSize.Width - position.Top(), position.Left()};
				b.TexCoord = texCoords.TopLeft();
				c.Position = {targetSize.Width - position.Top() - position.Height(), position.Right()};
				c.TexCoord = texCoords.BottomRight();
				d.Position = {targetSize.Width - position.Top(), position.Right()};
				d.TexCoord = texCoords.TopRight();
				break;
			case Filter::RotationAngle::Rotate90FlipY:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1),
											  static_cast<float>(targetSize.Height & 1)};
				// SCREENSPACE:
				// TopLeft vert (x, y)
				a.Position = {targetSize.Width - position.Top() - position.Height(),
							  targetSize.Height - position.Left() - position.Width()};
				a.TexCoord = texCoords.BottomRight();
				// TopRight vert (x, y)
				b.Position = {targetSize.Width - position.Top(),
							  targetSize.Height - position.Left() - position.Width()};
				b.TexCoord = texCoords.TopRight();
				c.Position = {targetSize.Width - position.Top() - position.Height(),
							  targetSize.Height - position.Left()};
				c.TexCoord = texCoords.BottomLeft();
				d.Position = {targetSize.Width - position.Top(), targetSize.Height - position.Left()};
				d.TexCoord = texCoords.TopLeft();
				break;
			case Filter::RotationAngle::Rotate180:
				adjustment += Geom::SizeFloat{static_cast<float>(targetSize.Width & 1),
											  static_cast<float>(targetSize.Height & 1)};
				a.Position = {targetSize.Width - position.Left() - position.Width(),
							  targetSize.Height - position.Top() - position.Height()};
				a.TexCoord = texCoords.BottomRight();
				b.Position = {targetSize.Width - position.Left(),
							  targetSize.Height - position.Top() - position.Height()};
				b.TexCoord = texCoords.BottomLeft();
				c.Position = {targetSize.Width - position.Left() - position.Width(),
							  targetSize.Height - position.Top()};
				c.TexCoord = texCoords.TopRight();
				d.Position = {targetSize.Width - position.Left(), targetSize.Height - position.Top()};
				d.TexCoord = texCoords.TopLeft();
				break;
			case Filter::RotationAngle::Rotate270:
				adjustment += Geom::SizeFloat{0, static_cast<float>(targetSize.Height & 1)};
				a.Position = {position.Top(), targetSize.Height - position.Left() - position.Width()};
				a.TexCoord = texCoords.TopRight();
				b.Position = {position.Bottom(), targetSize.Height - position.Left() - position.Width()};
				b.TexCoord = texCoords.BottomRight();
				c.Position = {position.Top(), targetSize.Height - position.Left()};
				c.TexCoord = texCoords.TopLeft();
				d.Position = {position.Bottom(), targetSize.Height - position.Left()};
				d.TexCoord = texCoords.BottomLeft();
				break;
			case Filter::RotationAngle::Rotate270FlipY:
				// SCREENSPACE:
				// TopLeft vert  (x, y)
				a.Position = {position.Top(), position.Left()};
				a.TexCoord = texCoords.TopLeft();
				// TopRight vert  (x, y)
				b.Position = {position.Bottom(), position.Left()};
				b.TexCoord = texCoords.BottomLeft();
				// Bottom Left (x, y)
				c.Position = {position.Top(), position.Right()};
				c.TexCoord = texCoords.TopRight();
				// Bottom Right (x, y)
				d.Position = {position.Bottom(), position.Right()};
				d.TexCoord = texCoords.BottomRight();
				break;
			default:
				DO_THROW(Err::InvalidParam, "Angle not supported");
				break;
		}
		a.Position += adjustment;
		b.Position += adjustment;
		c.Position += adjustment;
		d.Position += adjustment;
	}
}
