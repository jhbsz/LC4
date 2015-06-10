function todec(str) {
	hstr="0123456789ABCDEF";
	res=0;
	n=split(str,digit,"");
	for(i=1;i<=n;i++) { 
		num=index(hstr,digit[i])-1;
		res=res+(num*16^(n-i));
	}
	return res;
}

function tomac(x) {
	n=x;
	res="";
	for(i=1;i<=6;i++) {
		n=x/(256^(6-i))%256;
		x=n*(256^(6-i));
		if(i==1) {
			res=sprintf("%02X",n);
		}
		else {
			res=sprintf("%s:%02X",res,n);
		}
	    
	}
	return res;
}
