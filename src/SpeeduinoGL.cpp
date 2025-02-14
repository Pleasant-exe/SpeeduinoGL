#include "SpeeduinoGL.h"
#include <algorithm>

long ImageAddress = 0x60000000;
uint ResV = 480;
uint ResH = 800;

void ConfigBuffer(long address, uint ResolutionV, uint ResolutionH) {
ResV = ResolutionV;
ImageAddress = address;
    ResH = ResolutionH;
}

void FillTriangle(Triangle triangle, uint16_t Colour) {

  // Sort points based on x-coordinates
  std::sort(&triangle.A, &triangle.C + 1, [](const Point &a, const Point &b) {
    return a.w < b.w;
  });

  float gradAC = (triangle.A.h - triangle.C.h) / (triangle.A.w - triangle.C.w);
  float gradAB = (triangle.A.h - triangle.B.h) / (triangle.A.w - triangle.B.w);
  float gradBC = (triangle.B.h - triangle.C.h) / (triangle.B.w - triangle.C.w);
  bool Polarized = 0;

 if (gradAB > gradAC) {
  Polarized = 1;
 };

    PolarizedTwoLineRasterizer(ceil(triangle.A.w), ceil(triangle.B.w), triangle.A.h + gradAC * (ceil(triangle.A.w) - triangle.A.w), triangle.A.h + gradAB * (ceil(triangle.A.w) - triangle.A.w), gradAB, gradAC, Colour, Polarized);
    PolarizedTwoLineRasterizer(ceil(triangle.B.w), ceil(triangle.C.w), triangle.A.h + gradAC * (ceil(triangle.B.w) - triangle.A.w), triangle.B.h + gradBC * (ceil(triangle.B.w) - triangle.B.w), gradBC, gradAC, Colour, Polarized);
}

void FillSquare(Square square, uint16_t Colour) {

  // Sort points based on x-coordinates
  std::sort(&square.A, &square.C + 1, [](const Point &a, const Point &b) {
    return a.w < b.w;
  });

bool switched = 0;
bool Polarized = 0;

  float gradAC = (square.C.h - square.A.h) / (square.C.w - square.A.w);
   float gradAD = (square.D.h - square.A.h) / (square.D.w - square.A.w);
  float gradAB = (square.B.h - square.A.h) / (square.B.w - square.A.w);

  if (gradAD>gradAB) {
  if (gradAC>gradAD) {
std::swap(square.C, square.D);
std::swap(gradAC, gradAD);
switched = 1;
  } else {

  };
  } else {
  if (gradAC<gradAD) {
std::swap(square.C, square.D);
std::swap(gradAC, gradAD);
switched = 1;
  };
  }

  float gradBC = (square.C.h - square.B.h) / (square.C.w - square.B.w);
  float gradDC = (square.C.h - square.D.h) / (square.C.w - square.D.w);


 if (gradAB > gradAD) {
  Polarized = 1;
 };

    PolarizedTwoLineRasterizer(ceil(square.A.w), ceil(square.B.w), square.A.h + gradAD * (ceil(square.A.w) - square.A.w), square.A.h + gradAB * (ceil(square.A.w) - square.A.w), gradAB, gradAD, Colour, Polarized);
    if (switched) 
    {
    PolarizedTwoLineRasterizer(ceil(square.B.w), ceil(square.C.w), square.A.h + gradAD * (ceil(square.B.w) - square.A.w), square.B.h + gradBC * (ceil(square.B.w) - square.B.w), gradBC, gradAD, Colour, Polarized);
    PolarizedTwoLineRasterizer(ceil(square.C.w), ceil(square.D.w), square.A.h + gradAD * (ceil(square.C.w) - square.A.w), square.C.h + gradDC * (ceil(square.C.w) - square.C.w), gradDC, gradAD, Colour, Polarized);
    } else 
    {
    PolarizedTwoLineRasterizer(ceil(square.B.w), ceil(square.D.w), square.A.h + gradAD * (ceil(square.B.w) - square.A.w), square.B.h + gradBC * (ceil(square.B.w) - square.B.w), gradBC, gradAD, Colour, Polarized);
    PolarizedTwoLineRasterizer(ceil(square.D.w), ceil(square.C.w), square.D.h + gradDC * (ceil(square.D.w) - square.D.w), square.B.h + gradBC * (ceil(square.D.w) - square.B.w), gradBC, gradDC, Colour, Polarized);
    };
}

void PolarizedTwoLineRasterizer(int CellStartX, int CellEndX, float PointerCoordinateH, float PointerEndH, float Gradient1, float Gradient2, uint16_t Colour, bool Polarity) {
  if (Polarity) {
 TwoLineRasterizer(CellStartX, CellEndX, PointerCoordinateH, PointerEndH, Gradient1, Gradient2, Colour);
  } else {
 TwoLineRasterizer(CellStartX, CellEndX, PointerEndH, PointerCoordinateH, Gradient2, Gradient1, Colour);
  };
}

void TwoLineRasterizer(int CellStartX, int CellEndX, float PointerCoordinateH, float PointerEndH, float Gradient1, float Gradient2, uint16_t Colour) {

uint16_t *ImageBuffer = (uint16_t *)ImageAddress;
    
    if (CellStartX<0) {
        PointerCoordinateH -= Gradient2 * CellStartX;
        PointerEndH -= Gradient1 * CellStartX;
        CellStartX = 0;
    };
  if (CellEndX > ResH) {
      CellEndX = ResH;
  };
  for (uint CurrentW = CellStartX; CellEndX > CurrentW; CurrentW++) {
    uint PointerCoorInt = ceil(PointerCoordinateH);
    uint PointerEndInt = ceil(PointerEndH);
      
      if (PointerEndInt>ResV) {
          PointerEndInt = ResV;
      };

    for (int CurrentH = PointerCoorInt; PointerEndInt > CurrentH; CurrentH++) {
      ImageBuffer[ResV * (CurrentW) + (CurrentH)] = Colour;
    };
      
    PointerCoordinateH += Gradient2;
    PointerEndH += Gradient1;
  };
}
