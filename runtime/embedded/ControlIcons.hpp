#pragma once
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace lcom::embedded {
// Authored on a 24px grid, with consistent 2px strokes and round caps/joins.
inline void drawControlIcon(SDL_Renderer *renderer, const std::string &name,
                            float x, float y, uint32_t rgb, float scale=1) {
  struct Mesh { std::vector<SDL_Vertex> vertices; std::vector<int> indices; };
  static std::map<std::string,Mesh> icons;
  auto [entry,created]=icons.try_emplace(name);
  auto &mesh=entry->second;
  if (created) {
    auto fill=[&](const std::vector<SDL_FPoint> &points) {
      int base=static_cast<int>(mesh.vertices.size());
      for (auto p : points) mesh.vertices.push_back({p,{1,1,1,1},{0,0}});
      for (int i=1;i+1<static_cast<int>(points.size());++i) mesh.indices.insert(mesh.indices.end(),{base,base+i,base+i+1});
    };
    auto disc=[&](SDL_FPoint center, float radius) {
      std::vector<SDL_FPoint> points;
      for (int i=0;i<24;++i) {
        float angle=i*6.2831853f/24;
        points.push_back({center.x+radius*std::cos(angle),center.y+radius*std::sin(angle)});
      }
      fill(points);
    };
    auto stroke=[&](const std::vector<SDL_FPoint> &points) {
      for (size_t i=1;i<points.size();++i) {
        auto a=points[i-1], b=points[i];
        float length=std::hypot(b.x-a.x,b.y-a.y);
        if (!length) continue;
        float nx=(b.y-a.y)/length, ny=(a.x-b.x)/length;
        fill({{a.x+nx,a.y+ny},{b.x+nx,b.y+ny},{b.x-nx,b.y-ny},{a.x-nx,a.y-ny}});
      }
      for (auto point : points) disc(point,1);
    };
    auto arc=[&](float cx,float cy,float radius,float start,float end) {
      std::vector<SDL_FPoint> points;
      for (int i=0;i<=16;++i) {
        float angle=start+(end-start)*i/16;
        points.push_back({cx+radius*std::cos(angle),cy+radius*std::sin(angle)});
      }
      stroke(points);
    };
    if (name=="@left" || name=="@right" || name=="@up" || name=="@down") {
      float dx=name=="@left"?-1:name=="@right"?1:0;
      float dy=name=="@up"?-1:name=="@down"?1:0;
      stroke({{12-dx*8,12-dy*8},{12+dx*8,12+dy*8}});
      stroke({{12+dx*2-dy*6,12+dy*2+dx*6},{12+dx*8,12+dy*8},{12+dx*2+dy*6,12+dy*2-dx*6}});
    } else if (name=="@mouse-left" || name=="@mouse-right") {
      arc(12,8,6,3.1415927f,6.2831853f);
      arc(12,16,6,0,3.1415927f);
      stroke({{6,8},{6,16}}); stroke({{18,8},{18,16}});
      stroke({{12,3},{12,10}}); stroke({{6,10},{18,10}});
      disc({name=="@mouse-left"?9.f:15.f,7},1.6f);
    } else if (name=="@keyboard") {
      stroke({{4,5},{20,5},{22,7},{22,17},{20,19},{4,19},{2,17},{2,7},{4,5}});
      for (float yy : {9.f,12.f}) for (float xx : {6.f,10.f,14.f,18.f}) disc({xx,yy},0.85f);
      stroke({{8,16},{16,16}});
    } else if (name=="@hide" || name=="@controls") {
      float sign=name=="@hide"?1:-1;
      stroke({{6,12-3*sign},{12,12+3*sign},{18,12-3*sign}});
    } else if (name=="@stick") {
      arc(12,6,4,0,6.2831853f);
      stroke({{12,10},{12,17}});
      stroke({{8,14},{5,14},{3,21},{21,21},{19,14},{16,14}});
    } else if (name=="@move" || name=="@drag") {
      // A pointer is easier to recognise than the previous crosshair symbol.
      stroke({{5,3},{5,19},{9,15},{13,22},{16,20},{12,14},{18,14},{5,3}});
      if (name=="@drag") {
        stroke({{18,3},{18,8}}); stroke({{15.5f,5.5f},{20.5f,5.5f}});
      }
    }
  }
  // Build each vector mesh once. Each button takes one draw call per frame.
  auto vertices=mesh.vertices;
  SDL_FColor color{((rgb>>16)&255)/255.f,((rgb>>8)&255)/255.f,(rgb&255)/255.f,1};
  for (auto &vertex : vertices) { vertex.position.x=vertex.position.x*scale+x; vertex.position.y=vertex.position.y*scale+y; vertex.color=color; }
  SDL_RenderGeometry(renderer,nullptr,vertices.data(),static_cast<int>(vertices.size()),mesh.indices.data(),static_cast<int>(mesh.indices.size()));
}
}
