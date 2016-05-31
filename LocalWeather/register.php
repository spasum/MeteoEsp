<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <meta name="theme-color" content="#2A2D35">
    <meta name="msapplication-navbutton-color" content="#2A2D35"/>
    <meta name="apple-mobile-web-app-status-bar-style" content="#2A2D35"/>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <title>Домашняя метеостанция - Регистрация</title>
    <link rel="shortcut icon" href="favicon.ico" type="image/x-icon" />
    <link rel="stylesheet" href="styles/styles.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap-theme.min.css" type="text/css" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js" type="text/javascript"></script>
    <script src="scripts/CommonUtil.js" type="text/javascript"></script>
    <script src="scripts/queryHelper.js" type="text/javascript"></script>
    <script src="scripts/registerController.js" type="text/javascript"></script>
    <script src="scripts/bootstrap.min.js" type="text/javascript"></script>
    <script type="text/javascript">
        var timerStart = Date.now();
    </script>
</head>
<body>

<div class="container">
    <div class="row">
        <div class="absolute-center is-responsive">
            <div class="col-sm-12 col-md-12">
                <form method="post" id="loginForm">
                    <div class="form-group input-group" id="usernamePanel">
                        <span class="input-group-addon"><i class="glyphicon glyphicon-user"></i></span>
                        <input class="form-control" type="text" id="username" name='username' placeholder="e-mail"/>
                    </div>
                    <div class="form-group input-group" id="passwordPanel">
                        <span class="input-group-addon"><i class="glyphicon glyphicon-lock"></i></span>
                        <input class="form-control" type="password" id="password" name='password' placeholder="пароль"/>
                    </div>
                    <div class="form-group input-group" id="codePanel" style="display: none;">
                        <span class="input-group-addon"><i class="glyphicon glyphicon-check"></i></span>
                        <input class="form-control" type="text" id="code" name='code' placeholder="проверочный код"/>
                    </div>
                    <div class="form-group" id="registerPanel">
                        <button type="submit" id="btnRegister" class="btn btn-primary btn-block">Регистрация</button>
                    </div>
                    <div class="form-group" id="validationPanel" style="display: none;">
                        <button type="submit" id="btnValidate" class="btn btn-primary btn-block">Валидация</button>
                    </div>
                    <div class="form-group text-center">
                        <a href="login.php">Войти на сайт</a>&nbsp;|&nbsp;<a href="restorePassword.php">Восстановление пароля</a>
                    </div>
                    <div id="errorPane" class="alert alert-info" role="alert" style="visibility: hidden;"></div>
                </form>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    var registerPage;
    $(document).ready(function() {
        registerPage = registerController();
    });
</script>

</body>
</html>