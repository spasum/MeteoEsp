<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <meta name="theme-color" content="#2A2D35">
    <meta name="msapplication-navbutton-color" content="#2A2D35"/>
    <meta name="apple-mobile-web-app-status-bar-style" content="#2A2D35"/>
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <title>Домашняя метеостанция - Данные</title>
    <link rel="stylesheet" href="styles/styles.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/jquery.dropdown.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap-theme.min.css" type="text/css" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js" type="text/javascript"></script>
    <script src="scripts/CommonUtil.js" type="text/javascript"></script>
    <script src="scripts/queryHelper.js" type="text/javascript"></script>
    <script src="scripts/datasController.js" type="text/javascript"></script>
    <script src="scripts/bootstrap.min.js" type="text/javascript"></script>
    <script src="scripts/dateFormat.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="scripts/jquery.dropdown.min.js"></script>
    <script type="text/javascript">
        var timerStart = Date.now();
    </script>
</head>
<body>

<nav class="navbar navbar-default">
    <div class="container-fluid">
        <div class="navbar-header">
            <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
                <span class="sr-only">Показать меню</span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
                <span class="icon-bar"></span>
            </button>
            <a class="navbar-brand" href="/">Домашняя метеостанция</a>
        </div>
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav">
                <li><a href="/">Главная</a></li>
                <li class="active"><a href="/datas.php">Данные</a></li>
                <li><a href="/charts.php">Графики</a></li>
            </ul>
            <ul class="nav navbar-nav navbar-right">
                <li><a href="/setup.php">Настройки</a></li>
            </ul>
        </div>
    </div>
</nav>

<div class="pageContainer">

    <div class="panel-group">
        <div class="panel panel-default">
            <div class="panel-heading">
                <h4 class="panel-title">
                    <a data-toggle="collapse" href="#collapse1">Настроить параметры фильтрации &#8595;</a>
                </h4>
            </div>
            <div id="collapse1" class="panel-collapse collapse">
                <div class="panel-body">
                    <span class="example" data-jq-dropdown="#jq-dropdown-2">Фильтр данных</span>
                    <div id="jq-dropdown-2" class="jq-dropdown jq-dropdown-tip">
                        <ul class="jq-dropdown-menu" id="modulesFilterMenu">
                        </ul>
                    </div>
                </div>
            </div>
        </div>
    </div>

    <div id="results"></div>
    <div id="pager"></div>

    <div id="jq-dropdown-1" class="jq-dropdown jq-dropdown-tip">
        <ul class="jq-dropdown-menu">
            <li><a class="pageSizeItem">10</a></li>
            <li><a class="pageSizeItem">20</a></li>
            <li><a class="pageSizeItem">40</a></li>
        </ul>
    </div>
</div>

<script type="text/javascript">
    var datasPage;
    $(document).ready(function() {
        datasPage = datasController();
    });
</script>

</body>
</html>