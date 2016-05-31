
var registerController = function(params) {

    var userNameField = null;
    var passwordField = null;
    var btnRegister = null;
    var btnValidate = null;
    var errorPane = null;
    var codePanel = null;
    var codeField = null;
    var validationPanel = null;
    var usernamePanel = null;
    var passwordPanel = null;
    var registerPanel = null;

    function initData() {
        userNameField = ge("username");
        passwordField = ge("password");
        btnRegister = ge("btnRegister");
        btnValidate = ge("btnValidate");
        errorPane = ge("errorPane");
        codePanel = ge("codePanel");
        codeField = ge("code");
        validationPanel = ge("validationPanel");
        usernamePanel = ge("usernamePanel");
        passwordPanel = ge("passwordPanel");
        registerPanel = ge("registerPanel");
    }

    function disableForm(disable) {
        userNameField.disabled = passwordField.disabled = btnRegister.disabled = disable;
    }

    function validateForm(evt) {

        var errors = [];
        var email = userNameField.value;
        var password = passwordField.value;

        var emailIsValid = true;
        var passwordIsValid = true;

        errorPane.innerHTML = "";

        if (isStringEmpty(email) || !validateEmail(email)) {
            errors.push("Введите валидный e-mail");
            emailIsValid = false;
        }
        if (password.length < 6) {
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
            userNameField.focus();
        }
        else if (!passwordIsValid) {
            passwordField.focus();
        }

        errorPane.style.visibility = "visible";

        return false;
    }

    function registerUser() {
        var email = userNameField.value;
        var password = passwordField.value;
        queryHelper.requestUserData({ action: "register", email: email, password: password }, registerUserCallback);
    }

    function registerUserCallback(payload) {
        errorPane.style.visibility = "visible";

        if (payload.result === true) {
            errorPane.innerHTML = "На указанный e-mail отправлен проверочный код. Введите полученный код для окончания регистрации.";

            codePanel.style.display = "";
            codeField.focus();

            validationPanel.style.display = "";
            usernamePanel.style.display = passwordPanel.style.display = registerPanel.style.display = "none";

            btnRegister.type = "button";
        } else {
            disableForm(false);
            if (payload.alreadyRegistered) {
                errorPane.innerHTML = "Пользователь с указанным e-mail уже зарегистрирован. Введите другой e-mail.";
                userNameField.value = passwordField.value = "";
                userNameField.focus();
            }
        }
    }

    function doLogin(evt) {
        EventHelper.cancel(evt);
        if (validateForm(evt)) {
            disableForm(true);
            registerUser();
        }
    }

    function validateCode(evt) {
        EventHelper.cancel(evt);

        var code = codeField.value;
        if (isStringEmpty(code)) {
            errorPane.innerHTML = "Введите проверочный код.";
            codeField.focus();
            return;
        }

        queryHelper.requestUserData({ action: "validate", code: code }, validateUserCallback);
    }

    function validateUserCallback(payload) {

    }

    function init() {
        initData();
        userNameField.focus();
        btnRegister.onclick = doLogin;
        btnValidate.onclick = validateCode;
    }

    init();

};
