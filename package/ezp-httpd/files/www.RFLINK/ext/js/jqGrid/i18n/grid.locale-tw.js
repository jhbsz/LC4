;(function($){
/**
 * jqGrid Chinese Translation for v4.2
 * henryyan 2011.11.30
 * http://www.wsria.com
 * Dual licensed under the MIT and GPL licenses:
 * http://www.opensource.org/licenses/mit-license.php
 * http://www.gnu.org/licenses/gpl.html
 * 
 * update 2011.11.30
 *		add double u3000 SPACE for search:odata to fix SEARCH box display err when narrow width from only use of eq/ne/cn/in/lt/gt operator under IE6/7
**/
$.jgrid = $.jgrid || {};
$.extend($.jgrid,{
	defaults : {
		recordtext: "{0} - {1}\u3000共 {2} 筆",	// 共字前是全角空格
		emptyrecords: "無資料顯示",
		loadtext: "讀取中...",
		pgtext : " {0} 共 {1} 頁"
	},
	search : {
		caption: "搜索...",
		Find: "查詢",
		Reset: "重設",
		//odata : ['等於\u3000\u3000', '不等\u3000\u3000', '小於\u3000\u3000', '小於等於','大於\u3000\u3000','大於等於', 
		//	'開始於','不開始於','屬於\u3000\u3000','不屬於','結束於','不結束於','包含\u3000\u3000','不包含','空值于\u3000\u3000','非空值'],
		odata : ['等於', '不等於', '小於', '小於等於','大於','大於等於', '開始於','不開始於','屬於','不屬於','結束於','不結束於','包含','不包含'],
		groupOps: [	{ op: "AND", text: "所有" },	{ op: "OR",  text: "任一" }	],
		matchText: " 符合",
		rulesText: " 規定"
	},
	edit : {
		addCaption: "增加資料",
		editCaption: "編輯資料",
		bSubmit: "送出",
		bCancel: "取消",
		bClose: "關閉",
		saveData: "資料已修改，是否存檔?",
		bYes : "是",
		bNo : "否",
		bExit : "取消",
		msg: {
			required:"此為必填欄位",
			number:"請輸入有效數字",
			minValue:"輸入值必須大於等於",
			maxValue:"輸入值必須小於等於",
			email: "非有效E-mail",
			integer: "請輸入整數",
			date: "請輸入有效時間",
			url: "無效網址。前段需要 ('http://' 或 'https://')",
			nodefined : " 未定義！",
			novalue : " 需有回應值！",
			customarray : "自定函數需要有回傳陣列值！",
			customfcheck : "Custom function should be present in case of custom checking!"
			
		}
	},
	view : {
		caption: "查看資料",
		bClose: "關閉"
	},
	del : {
		caption: "刪除",
		msg: "刪除所選資料?",
		bSubmit: "刪除",
		bCancel: "取消"
	},
	nav : {
		edittext: "",
		edittitle: "修改所選資料",
		addtext:"",
		addtitle: "增加新資料",
		deltext: "",
		deltitle: "刪除所選資料",
		searchtext: "",
		searchtitle: "查詢",
		refreshtext: "",
		refreshtitle: "重整表格",
		alertcap: "注意",
		alerttext: "請選擇資料",
		viewtext: "",
		viewtitle: "查看所選資料"
	},
	col : {
		caption: "選擇欄位",
		bSubmit: "確定",
		bCancel: "取消"
	},
	errors : {
		errcap : "錯誤",
		nourl : "沒有設定URL",
		norecords: "沒有須處理資料",
		model : "colNames 和 colModel 長度不等!"
	},
	formatter : {
		integer : {thousandsSeparator: " ", defaultValue: '0'},
		number : {decimalSeparator:".", thousandsSeparator: " ", decimalPlaces: 2, defaultValue: '0.00'},
		currency : {decimalSeparator:".", thousandsSeparator: " ", decimalPlaces: 2, prefix: "", suffix:"", defaultValue: '0.00'},
		date : {
			dayNames:   [
				"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat",
		         "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
			],
			monthNames: [
				"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec",
				"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
			],
			AmPm : ["am","pm","AM","PM"],
			S: function (j) {return j < 11 || j > 13 ? ['st', 'nd', 'rd', 'th'][Math.min((j - 1) % 10, 3)] : 'th'},
			srcformat: 'Y-m-d',
			newformat: 'm-d-Y',
			masks : {
				ISO8601Long:"Y-m-d H:i:s",
				ISO8601Short:"Y-m-d",
				ShortDate: "Y/j/n",
				LongDate: "l, F d, Y",
				FullDateTime: "l, F d, Y g:i:s A",
				MonthDay: "F d",
				ShortTime: "g:i A",
				LongTime: "g:i:s A",
				SortableDateTime: "Y-m-d\\TH:i:s",
				UniversalSortableDateTime: "Y-m-d H:i:sO",
				YearMonth: "F, Y"
			},
			reformatAfterEdit : false
		},
		baseLinkUrl: '',
		showAction: '',
		target: '',
		checkbox : {disabled:true},
		idName : 'id'
	}
});
})(jQuery);
