
var queryHelper = new function() {

    var thisRef = this;

    function request(url, params, callback) {
        var request = $.ajax({
            url: url,
            type: "post",
            data: params
        });

        request.done(function (response){
            var payload = JSON.parse(response);
            callback(payload);
        });

        // Callback handler that will be called on failure
        request.fail(function (jqXHR, textStatus, errorThrown){
            console.error(
                "The following error occurred: " +
                textStatus, errorThrown
            );
        });
    }

    function updateSensorData(params, callback) {
        request("updateSensorData.php", params, callback);
    }
    thisRef.updateSensorData = updateSensorData;

    function requestSensorData(params, callback) {
        request("querySensorData.php", params, callback);
    }
    thisRef.requestSensorData = requestSensorData;

    function requestModuleData(params, callback) {
        request("queryModuleData.php", params, callback);
    }
    thisRef.requestModuleData = requestModuleData;

    function requestWeatherData(params, callback) {
        request("queryWeatherData.php", params, callback);
    }
    thisRef.requestWeatherData = requestWeatherData;
};
