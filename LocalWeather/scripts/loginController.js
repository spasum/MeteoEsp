
var loginController = function(params) {

    function validateForm(evt) {
        var errors = [];
        var email = ge("username").value;
        var password = ge("password").value;

        var errorPane = ge("errorPane");
        errorPane.innerHTML = "";

        if (isStringEmpty(email)) {
            errors.push("Введите e-mail");
        }
        if (isStringEmpty(password)) {
            errors.push("Введите пароль");
        }

        if (errors.length == 0) {
            errorPane.style.visibility = "hidden";
            return true;
        }

        for (var i = 0; i < errors.length; i++) {
            errorPane.innerHTML += errors[i];
            if (i != errors.length) {
                errorPane.innerHTML += "<br/>";
            }
        }

        if (isStringEmpty(email)) {
            ge("username").focus();
        }
        else if (isStringEmpty(password)) {
            ge("password").focus();
        }

        EventHelper.cancel(evt);
        errorPane.style.visibility = "visible";

        return false;
    }

    function doLogin(evt) {
        if (!validateForm(evt)) {
        }
    }

    function init() {
        ge("username").focus();
        ge("btnLogin").onclick = doLogin;
    }

    init();

};
