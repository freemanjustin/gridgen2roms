var raster = new ol.layer.Tile({
    source: new ol.source.MapQuest({layer: 'sat'})
});

var map = new ol.Map({
    layers: [raster],
    target: 'map',
    view: new ol.View({
	center: [-25242564.2, -2856910.35],
	zoom: 3
    })
});

var features = new ol.Collection();
var featureOverlay = new ol.layer.Vector({
    source: new ol.source.Vector({features: features}),
    style: new ol.style.Style({
	fill: new ol.style.Fill({
	    color: 'rgba(255, 255, 255, 0.2)'
	}),
	stroke: new ol.style.Stroke({
	    color: '#ffcc33',
	    width: 2
	}),
	image: new ol.style.Circle({
	    radius: 7,
	    fill: new ol.style.Fill({
		color: '#ffcc33'
	    })
	})
    })
});
featureOverlay.setMap(map);

var modify = new ol.interaction.Modify({
    features: features,
    // the SHIFT key must be pressed to delete vertices, so
    // that new vertices can be drawn at the same position
    // of existing vertices
    deleteCondition: function(event) {
	return ol.events.condition.shiftKeyOnly(event) &&
            ol.events.condition.singleClick(event);
    }
});

modify.on('modifyend', function (event) {
    console.info("modifyend", features.getArray());
    outputFeaturescoords(true);
});



map.addInteraction(modify);

function round(num, dp) {
    return Math.round(num*10*dp)/(10*dp);
}

function outputFeaturescoords(clear) {

    var fts = featureOverlay.getSource().getFeatures();
    console.info("fts", fts);

    if (clear) {
	$("#feature_coords").html("");
    }
    var str = $("#feature_coords").html();

    for (i in fts) {
	//var feature = event.element;
	var coords = fts[i].getGeometry().getCoordinates();


	for (k in coords) {
	    for (coord in coords[k]) {
		//coords[k][coord] = ol.proj.transform([coords[k][coord][0], coords[k][coord][1]], 'EPSG:3857', 'EPSG:4326');
		coords[k][coord] = ([coords[k][coord][0], coords[k][coord][1]]);
                str += round(coords[k][coord][0],2)+" "+round(coords[k][coord][1],2)+"  <br/>";
	    }
	}
	str += "================<br/>";
    }
    $("#feature_coords").html(str);
}
function output_coords(event) {
    var str = "";

    //var feature = event.element;
    var coords = event.feature.getGeometry().getCoordinates();
    console.info(coords);

    for (k in coords) {
	for (coord in coords[k]) {
	    //coords[k][coord] = ol.proj.transform([coords[k][coord][0], coords[k][coord][1]], 'EPSG:3857', 'EPSG:4326');
	    coords[k][coord] = ([coords[k][coord][0], coords[k][coord][1]]);
            str += round(coords[k][coord][0],2)+" "+round(coords[k][coord][1],2)+" <br/>";
	}
	str += "================<br/>";
    }
    $("#feature_coords").html(str);
}


var draw; // global so we can remove it later
function addInteraction() {
    draw = new ol.interaction.Draw({
	features: features,
	type: /** @type {ol.geom.GeometryType} */ (typeSelect.value)
    });

    draw.on('drawend', function (event) {
	// get the feature
	//console.info("drawend", featureOverlay.getSource().getFeatures());
	output_coords(event);
	outputFeaturescoords(false);

    });


    map.addInteraction(draw);
}

var typeSelect = document.getElementById('type');


/**
 * Let user change the geometry type.
 * @param {Event} e Change event.
 */
typeSelect.onchange = function(e) {
    map.removeInteraction(draw);
    addInteraction();
};

addInteraction();
