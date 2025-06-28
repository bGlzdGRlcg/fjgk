String.prototype.trim = function () {
	return this.replace(/　/g, " ").replace(/ /g, "");
};

var pattern = /^\d{1,}$/;

function init() {
	var err = "";
	if (err != "") {
		if (err == "1") {
			alert("账号、密码或考生号错误");
			document.getElementById("pwd").value = "";
			return false;
		} else if (err == "2") {
			alert("验证码输入错误");
			document.getElementById("pwd").value = "";
			return false;
		}
	}
}

function rest() {
	document.getElementById("ksh").value = "";
	document.getElementById("pwd").value = "";
	document.getElementById("logname").value = "";
	hideMessages();
}

function find(evnt) {
	var myEvent = evnt || window.event;
	if (!myEvent) {
		window.location = "http://www.eeafj.cn/";
		return;
	}

	document.getElementById("ksh").value = document
		.getElementById("ksh")
		.value.trim();

	var quanjiao = /[\uff00-\uffff]/g;
	var hanzi = /[^\x00-\xff]/g;

	if (document.getElementById("logname").value == "") {
		showError("账号不为空");
		return false;
	} else if (
		quanjiao.test(document.getElementById("logname").value) ||
		hanzi.test(document.getElementById("logname").value)
	) {
		showError("账号不能包含全角字符，请核实。");
		return false;
	}

	if (document.getElementById("pwd").value == "") {
		showError("密码不为空");
		return false;
	} else if (
		quanjiao.test(document.getElementById("pwd").value) ||
		hanzi.test(document.getElementById("pwd").value)
	) {
		showError("密码不能包含全角字符，请核实。");
		return false;
	}

	if (document.getElementById("ksh").value == "") {
		showError("考生号不为空");
		return false;
	} else if (!pattern.test(document.getElementById("ksh").value)) {
		showError("考生号请输入数字");
		return false;
	}

	startQuery();
}

function startQuery() {
	const findbtn = document.getElementById("findbtn");
	const restbtn = document.getElementById("restbtn");
	const loadingText = document.getElementById("loadingText");

	findbtn.disabled = true;
	findbtn.value = "正在查询";
	restbtn.style.display = "none";
	loadingText.style.display = "block";
	hideMessages();

	const logname = document.getElementById("logname").value;
	const password = document.getElementById("pwd").value;
	const ksh = document.getElementById("ksh").value;

	const passwordMd5 = MD5(password).toLowerCase();

	document.getElementById("pwd").value = passwordMd5;

	document.forms[0].submit();
}

function showError(message) {
	const errorMessage = document.getElementById("errorMessage");
	errorMessage.textContent = message;
	errorMessage.style.display = "block";

	setTimeout(() => {
		errorMessage.style.display = "none";
	}, 3000);
}

function showSuccess(message) {
	const successMessage = document.getElementById("successMessage");
	successMessage.textContent = message;
	successMessage.style.display = "block";
}

function hideMessages() {
	document.getElementById("errorMessage").style.display = "none";
	document.getElementById("successMessage").style.display = "none";
}

document.addEventListener("DOMContentLoaded", function () {
	init();

	document.addEventListener("keypress", function (e) {
		if (e.key === "Enter") {
			find(e);
		}
	});
});
