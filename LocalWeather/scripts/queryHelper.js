
var queryHelper = new function() {

    var thisRef = this;

    function requestSensorData(params, callback) {
        var request = $.ajax({
            url: "querySensorData.php",
            type: "post",
            data: {
            }
        });

        request.done(function (response){
            var sensorsData = JSON.parse(response);
            callback(sensorsData);
        });

        // Callback handler that will be called on failure
        request.fail(function (jqXHR, textStatus, errorThrown){
            console.error(
                "The following error occurred: " +
                textStatus, errorThrown
            );
        });
    }
    thisRef.requestSensorData = requestSensorData;

    function requestModuleData(params, callback) {
        var request = $.ajax({
            url: "queryModuleData.php",
            type: "post",
            data: {
            }
        });

        request.done(function (response){
            var moduleData = JSON.parse(response);
            callback(moduleData);
        });

        // Callback handler that will be called on failure
        request.fail(function (jqXHR, textStatus, errorThrown){
            console.error(
                "The following error occurred: " +
                textStatus, errorThrown
            );
        });
    }
    thisRef.requestModuleData = requestModuleData;

    function requestWeatherData(params, callback) {
        var request = $.ajax({
            url: "queryWeatherData.php",
            type: "post",
            data: {
                sortBy: params.sortBy,
                sortAscending: params.sortAscending,
                pageSize: params.pageSize,
                pageIndex: params.pageIndex,
                queryType: params.queryType,
                interval: params.interval
            }
        });

        request.done(function (response){
            var weatherData = JSON.parse(response);
            for (var i = 0; i < weatherData.data.length; i++) {
                weatherData.data[i].MeasuredDateTime = new Date(weatherData.data[i].MeasuredDateTime);
            }
            callback(weatherData);
        });

        // Callback handler that will be called on failure
        request.fail(function (jqXHR, textStatus, errorThrown){
            console.error(
                "The following error occurred: " +
                textStatus, errorThrown
            );
        });
    }
    thisRef.requestWeatherData = requestWeatherData;

};
