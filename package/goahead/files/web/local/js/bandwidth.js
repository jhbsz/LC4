$(document).ready(
	function () {
		$("#web").hover(

		function(){
				$("#web td:eq(1)").attr({"background":"images/b_web_on.gif"});
				$("#web td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#web td:eq(1)").attr({"background":"images/b_web.gif"});
				$("#web td:eq(2)").attr({"background":"images/b_right.gif"});

			}
		);

		/*$("#email").hover(
			function(){
				$("#email td:eq(1)").attr({"background":"images/b_mail_on.gif"});
				$("#email td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#email td:eq(1)").attr({"background":"images/b_mail.gif"});
				$("#email td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);*/
		$("#mail").hover(
			function(){
				$("#mail td:eq(1)").attr({"background":"images/b_mail_on.gif"});
				$("#mail td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#mail td:eq(1)").attr({"background":"images/b_mail.gif"});
				$("#mail td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);

		$("#voip").hover(
			function(){
				$("#voip td:eq(1)").attr({"background":"images/b_voip_on.gif"});
				$("#voip td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#voip td:eq(1)").attr({"background":"images/b_voip.gif"});
				$("#voip td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);

		/*$("#mstreaming").hover(
			function(){
				$("#mstreaming td:eq(1)").attr({"background":"images/b_media_on.gif"});
				$("#mstreaming td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#mstreaming td:eq(1)").attr({"background":"images/b_media.gif"});
				$("#mstreaming td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);*/
		$("#media").hover(
			function(){
				$("#media td:eq(1)").attr({"background":"images/b_media_on.gif"});
				$("#media td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#media td:eq(1)").attr({"background":"images/b_media.gif"});
				$("#media td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);


		$("#p2p").hover(
			function(){
				$("#p2p td:eq(1)").attr({"background":"images/b_p2p_on.gif"});
				$("#p2p td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#p2p td:eq(1)").attr({"background":"images/b_p2p.gif"});
				$("#p2p td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);

		$("#game").hover(
			function(){
				$("#game td:eq(1)").attr({"background":"images/b_game_on.gif"});
				$("#game td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#game td:eq(1)").attr({"background":"images/b_game.gif"});
				$("#game td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);
				$("#others").hover(
			function(){
				$("#others td:eq(1)").attr({"background":"images/b_others_on.gif"});
				$("#others td:eq(2)").attr({"background":"images/b_right_on.gif"});
			},
			function(){
				$("#others td:eq(1)").attr({"background":"images/b_others.gif"});
				$("#others td:eq(2)").attr({"background":"images/b_right.gif"});
			}
		);

		$('div.groupWrapper').Sortable(
			{
				accept: 'groupItem',
				helperclass: 'sortHelper',
				activeclass : 	'sortableactive',
				hoverclass : 	'sortablehover',
				tolerance: 'pointer',
				onChange : function(ser)
				{
				},
				onStart : function()
				{
					$.iAutoscroller.start(this, document.getElementsByTagName('body'));
				},
				onStop : function()
				{
					record_pos();
					$.iAutoscroller.stop();
				}
			}
		);
	}
);
var toggleContent = function(e)
{
	var targetContent = $('div.itemContent', this.parentNode.parentNode);
	if (targetContent.css('display') == 'none') {
		targetContent.slideDown(300);
		$(this).html('[-]');
	} else {
		targetContent.slideUp(300);
		$(this).html('[+]');
	}
	return false;
};
function serialize(s)
{
	serial = $.SortSerialize(s);
	alert(serial.hash);
};