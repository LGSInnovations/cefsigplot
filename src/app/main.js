let sigplot = require("sigplot");
let options = {};
let plot = new sigplot.Plot(document.getElementById('plot'), options);

function overlay_href(href) {
  plot.overlay_href(href);
}

window.overlay_href = overlay_href;
