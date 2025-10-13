#include "../sciplot/sciplot/Canvas.hpp"
#include "../sciplot/sciplot/Constants.hpp"
#include "../sciplot/sciplot/Figure.hpp"
#include "../sciplot/sciplot/Plot2D.hpp"
#include "../sciplot/sciplot/sciplot.hpp"
#include <vector>
using namespace sciplot;
using namespace std;

void make_graph(vector<int> x, vector<double> y) {
  Plot2D plot;
  plot.drawCurveFilled(x, y).above().fillColor("orange").lineWidth(6);
  plot.palette("paired");

  plot.xlabel("Word");
  plot.ylabel("WPM");

  Figure fig = {{plot}};
  fig.title("WPM");

  Canvas canvas = {{fig}};
  canvas.size(1000, 500);
  canvas.save("plot_output.png");
}
