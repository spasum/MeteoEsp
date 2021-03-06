
var indexController = function(params) {

    var SENSORS_DELAY = 10000;

    function requestModulesData() {
        queryHelper.requestModuleData({ sortBy: "ModuleName" }, renderModulesData);
    }

    function renderModulesData(moduleData) {

        var fields = moduleData.fields;
        var modules = moduleData.data;

        if (modules.length > 0) {
            ge("jumboMessage").style.display = "none";
        }

        for (var i = 0; i < modules.length; i++) {
            modules[i].LastSeenDateTime = new Date(modules[i].LastSeenDateTime);
        }

        var container = ge("pageContainer");
        var row = document.createElement("div");
        row.className = "row";
        container.appendChild(row);

        for (i = 0; i < modules.length; i++) {
            renderModule(row, fields, modules[i]);
        }

        window.setInterval(onTimer, SENSORS_DELAY);

        console.log("Time until everything loaded: ", Date.now()-timerStart);
    }

    function getModuleStatus(module) {
        var isActive = module.IsActive !== 0;
        var lastSeenDateTime = module.LastSeenDateTime;
        var diff = Math.abs(new Date() - lastSeenDateTime);
        var delay = module.SensorDelay * 1000; //in ms
        var isOn = diff < delay * 2;
        var status = isActive ? (isOn ? "В сети" : "Не в сети") : "Выключен";
        var className = isOn ? "success" : "danger";

        var headerId = "{0}_header".format(module.MAC);
        var widgetId = "{0}_widget".format(module.MAC);

        return {
            name: module.ModuleName,
            moduleId: module.ModuleID,
            description: isStringEmpty(module.Description) ? "{0} (#{1})".format(module.ModuleName, module.ModuleID) : module.Description,
            status: status,
            className: className,
            isOn: isOn,
            headerId: headerId,
            widgetId: widgetId
        };
    }

    function getModuleTitle(status) {
        return {
            header: "{0}&nbsp;&nbsp;<span class='label label-{2}'>{1}</span>".format(status.description, status.status, status.className),
            title: "{0} (#{1})".format(status.name, status.moduleId)
        };
    }

    function renderModule(row, fields, module) {

        var status = getModuleStatus(module);

        var col = document.createElement("div");
        col.id = status.widgetId;
        col.className = "col-sm-6 col-md-4 moduleWidget";
        if (!status.isOn) {
            col.classList.add("inactiveModuleWidget");
        }
        row.appendChild(col);

        var thumbnail = document.createElement("div");
        thumbnail.className = "thumbnail";
        col.appendChild(thumbnail);

        var caption = document.createElement("div");
        caption.className = "caption";
        thumbnail.appendChild(caption);

        var header = document.createElement("h3");
        header.id = status.headerId;
        var title = getModuleTitle(status);
        header.innerHTML = title.header;
        header.title = title.title;
        caption.appendChild(header);

        var hr = document.createElement("hr");
        caption.appendChild(hr);
    }

    function onTimer() {
        queryHelper.requestModuleData({}, updateModulesData);
    }

    function updateModulesData(moduleData) {

        var fields = moduleData.fields;
        var modules = moduleData.data;

        for (var i = 0; i < modules.length; i++) {
            modules[i].LastSeenDateTime = new Date(modules[i].LastSeenDateTime);
        }

        for (i = 0; i < modules.length; i++) {
            updateModule(fields, modules[i]);
        }
    }

    function updateModule(fields, module) {

        var status = getModuleStatus(module);

        var col = ge(status.widgetId);
        if (status.isOn) {
            col.classList.remove("inactiveModuleWidget");
        } else {
            col.classList.add("inactiveModuleWidget");
        }

        var header = ge(status.headerId);
        var title = getModuleTitle(status);
        header.innerHTML = title.header;
        header.title = title.title;
    }

    function init() {
        requestModulesData();
    }

    init();
};
