
var registerController = function(params) {

    function disableForm(disable) {
        ge("username").disabled = ge("password").disabled = ge("btnRegister").disabled = disable;
    }

    function validateForm(evt) {

        var errors = [];
        var email = ge("username").value;
        var password = ge("password").value;

        var emailIsValid = true;
        var passwordIsValid = true;

        var errorPane = ge("errorPane");
        errorPane.innerHTML = "";

        if (isStringEmpty(email) || !validateEmail(email)) {
            errors.push("Введите валидный e-mail");
            emailIsValid = false;
        }
        if (isStringEmpty(password)) {
            errors.push("Введите пароль (минимум 6 символов)");
            passwordIsValid = false;
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

        if (!emailIsValid) {
            ge("username").focus();
        }
        else if (!passwordIsValid) {
            ge("password").focus();
        }

        EventHelper.cancel(evt);
        errorPane.style.visibility = "visible";

        return false;
    }

    function doLogin(evt) {
        if (validateForm(evt)) {
            disableForm(true);
        }
    }

    function init() {
        ge("username").focus();
        ge("btnRegister").onclick = doLogin;
    }

    init();

};
