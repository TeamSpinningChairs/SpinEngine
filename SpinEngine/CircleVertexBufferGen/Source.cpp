#include <iostream>
#include<cmath>

int main()
{
  int line_count = 60;

  float angle_diff = (3.14159265358979323846 *2.0f) / static_cast<float>(line_count);
  std::cout << "angle_diff = " << angle_diff<<"\n\n";

  std::cout << "d3dVertex::VertexPositionColor circle_debug_draw_verts[" << line_count + 1<<"] =\n{\n";
  for (int i = 0; i < line_count; ++i)
  {
    std::cout << "d3dVertex::VertexPositionColor(" << cos(angle_diff * static_cast<float>(i)) << "," << sin(angle_diff * static_cast<float>(i)) << ", 0.0f, debug_draw_collision_wireframe_color)";
    
    if (i !=  line_count)
    {
      std::cout << ", \n";
    }
  }

  std::cout << "d3dVertex::VertexPositionColor(" << cos(0.0f) << "," << sin(0.0f) << ", 0.0f, debug_draw_collision_wireframe_color)";

  std::cout << "\n};";

  return 0;
}