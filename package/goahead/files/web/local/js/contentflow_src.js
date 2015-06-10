/*  ContentFlow, version 0.5.3 
 *  (c) 2007, 2008 Sebastian Kutsch
 *  <http://www.jacksasylum.eu/ContentFlow/>
 *
 *  ContentFlow is distributed under the terms of the MIT license.
 *  (see http://www.jacksasylum.eu/ContentFlow/LICENSE)
 *
 *--------------------------------------------------------------------------*/
var ContentFlow = function (container, config, initOnLoad) {
    if (container) {
        this.container = container;
        var init = this.init.cfBind(this);
        if (initOnLoad == undefined || initOnLoad == true) {
            this._addEvent(window, 'load', init, false);
        }
        for (var option in this.defaultConf) {
            this[option] = this.defaultConf[option];
        }
        this.setConfig(config?config:{});
    } else {
        throw ('ContentFlow ERROR: No flow container node or id given');
    }


};

ContentFlow.prototype = {
    _activeItem: 0,
    _currentPos: 0,
    _targetItem: 0,
    _stepLock: false,
    _millisecondsPerStep: 50, 
    _fileRegEx: /(([^\/?=&]+)\.(\w+)){1}$/,
    _browserIE: (document.all && !window.opera) ? true : false,
    _browserIE7: (this._browserIE && typeof(document.body.style.maxHeight) != "undefined") ? true : false,
    _browserKonq: navigator.appName.indexOf('Konqueror') != -1 ? true : false,
   
    defaultConf: { 
	
		
        biggestItemPos: 0,
        loadingTimeout: 30000, //milliseconds

        maxItemHeight: 0,
        scaleFactor: 1.15,
        scaleFactorLandscape: 0.9,

        visibleItems: -1,
        startItem:  "center",
        scrollInFrom: "pre",

        flowSpeedFactor: 1.0,
        flowDragFriction: 1.0,
        scrollWheelSpeed: 1.0,

        reflectionType: "none",   // client-side, server-side, none
        reflectionWithinImage: true,
        reflectionColor: "transparent", // none, transparent, overlay or hex RGB CSS style #RRGGBB
        reflectionHeight: 0,          // float (relative to original image height)
        negativeMarginOnFloat: "auto",  // auto, none or float (relative to reflectionHeight)
        reflectionOverlaySrc: "images/reflectionOverlay.png",
        reflectionServerSrc: "{URLTO}{FILENAME}_reflection.{EXT}"


    },


    /*
     * ==================== public methods ==================== 
     */

    /*
     * calls _init() if ContentFlow has not been initialized before
     * needed if ContentFlow is not automatically initialized in window.load
     */
    init: function () {
        if(this.isInit) return;
        this._init();
    },

    /*
     * returns a data object, containing all configuration parameters and their values
     */
    getConfig: function () {
        var conf = {};
        for (var option in this.defaultConf) {
            conf[option] = this[option];
        }
        return conf;
    },

    /*
     * parses configuration object and initializes configuration values
     */
    setConfig: function(config) {
        if (!config) return;
        var dC = this.defaultConf;
        for (var option in config) {
            if (dC[option] == "undefined" ) continue;
            switch (option) {
                case "scrollInFrom":
                case "startItem":
                case "negativeMarginOnFloat":
                    if (typeof(config[option]) == "number"  || typeof(config[option]) == "string") {
                        this[option] = config[option];
                    }
                    break;
                default:
                    if (typeof(dC[option] == config[option])) {
                        this[option] = config[option];
                    }
            }
        }
        switch (this.reflectionColor) {
            case "overlay":
                break;
            case this.reflectionColor.search(/#[0-9a-fA-F]{6}/)>= 0?this.reflectionColor:this.reflectionColor+"x":
                this.reflectionColorRGB = {
                    hR: this.reflectionColor.slice(1,3),
                    hG: this.reflectionColor.slice(3,5),
                    hB: this.reflectionColor.slice(5,7),
                    iR: parseInt(this.reflectionColor.slice(1,3), 16),
                    iG: parseInt(this.reflectionColor.slice(3,5), 16),
                    iB: parseInt(this.reflectionColor.slice(5,7), 16)
                };
                break;
            case "none":
            case "transparent":
            default:
                this.reflectionColor = "transparent"; 
                this.reflectionColorRGB = {
                    hR: 0, hG: 0, hB:0,
                    iR: 0, iG: 0, iB:0
                };
                break;
        }
        if (this.negativeMarginOnFloat == "none") this.negativeMarginOnFloat = 0;
        if (this.items) {
            if (this.visibleItems <  0)
                this.visibleItems = Math.round(Math.sqrt(this.items.length));
            this.visibleItems = Math.min(this.visibleItems, this.items.length - 1);
        }

    },

    /*
     * returns the index number of the active item
     */
    getActiveItem: function() {
        return this._activeItem;
    },

    /*
     * returns the number of items the flow contains
     */
    getNumberOfItems: function () {
        return this.items.length;
    },

    /*
     * reinitializes sizes.
     * called on window.resize
     */
    resize: function () {
        this._initSizes();
        this._initStep();
    }, 
    
    /*
     * scrolls flow to item i
     */
    moveTo: function (i) {
        this._targetItem = this._getIndexByKeyWord(i);
        this._initStep();
    }, 

    /*
     * initializes item and adds it at index position
     */
    addItem: function(el, index) {
        if (index == "undefined") index = this._activeItem;
        else if (typeof index == "string") {
            switch (index) {
                case "first":
                case "start":
                    index = 0;
                    break;
                case "last":
                case "end":
                    index = this.itemsLastIndex + 1;
                    break;
                default:
                    index = this._activeItem;
            }
        }
        index = Math.max(index, 0);
        index = Math.min(index, this.itemsLastIndex + 1);
        
        /* insert item at index position into flow */ 
        if (index > this.itemsLastIndex || !this.items[index])
            this.flow.appendChild(el);
        else
            this.flow.insertBefore(el, this.items[index]);

        if (this._activeItem < 0 ) {
            this._addClassName(el, 'active');
        }

        /* prepare arrays */
        this.items.splice(index,0, el);
        this.itemsContent.splice(index,0, false);
        this.itemsCaption.splice(index,0, false);
        this.itemsContentImage.splice(index, 0, false);
        this.itemsReflection.splice(index,0, false);
        this.itemsReflectionOverlay.splice(index, 0, false);
        this.itemsOverlay.splice(index, 0, false);
       
        /* init item after insertion and array preparation to be sure that it is part of the document */
        this._initItem(index);

        /* adjust item indices */
        for (var i = index ; i < this.items.length; i++) {
            this.items[i].itemIndex = i;
        }

        this._setLastIndex();

        /* adjust targetItem, currentPos and activeItem so that current view does not change*/
        this._targetItem = Math.max(this._targetItem, 0);
        this._currentPos = Math.max(this._currentPos, 0);
        this._activeItem = Math.max(this._activeItem, 0);
        if (index <= this._targetItem) {
            this._targetItem = Math.min(++this._targetItem, this.itemsLastIndex);
        } 
        if (index <= this._currentPos) {
            this._currentPos = Math.min(++this._currentPos, this.itemsLastIndex);
        }
        if (index <= this._activeItem) {
            this._activeItem = Math.min(++this._activeItem, this.itemsLastIndex);
        }
        
        this._initStep();
        return index;
        
    },
    
    /*
     * removes item at index position, cleans it up and returns it
     */
    rmItem: function(index) {
        if  (index == "undefined" || index == "undifined") index = this._activeItem;
        index = this._checkIndex(index);
        if (!this.items[index]) return null;

        /* remove event listeners */
        var fcii = this._fcii;
        var fcai = this._fcai;
        if (window.removeEventListener) {
            this._rmEvent(this.items[index], 'click', fcii, false);
            this._rmEvent(this.items[index], 'click', fcai, false);
        } else {
            this._rmEvent(this.items[index], 'click', fcii, false);
            this.items[index].onclick = function () {};
        }

        this.items[index].style.height = "";
        this.items[index].style.width = "";
        this.items[index].style.margin = "";
        this.items[index].style.top = "";
        this.items[index].style.left = "";
        this.items[index].style.fontSize = "";
        this.items[index].style.zIndex = "";
        this.items[index].style.display = "";

        /* remove classes */
        this._removeClassName(this.items[index], 'active');
        this._removeClassName(this.items[index], 'withReflection');
        if (this.itemsContentImage[index]) {
            this._removeClassName(this.itemsContent[index], 'portray');
            this._removeClassName(this.itemsContent[index], 'landscape');
        }
        
        /* cleanup arrays and remove generated content */
        var item = this.items[index];
        if (this.itemsContentImage[index]) {
            //this.itemsContent[index].width = this.itemsContent[index].origWidth;
            //this.itemsContent[index].height = this.itemsContent[index].origHeight;
            this.itemsContent[index].removeAttribute('width');
            this.itemsContent[index].removeAttribute('height');
            this.itemsContent[index].style.width = "";
            this.itemsContent[index].style.height = "";
            this.itemsContent[index].style.margin = "";

            this.itemsContentImage.splice(index, 1);
            if (this.itemsReflection[index]) {
                item.removeChild(this.itemsReflection[index]);
                this.itemsReflection.splice(index,1);

                if (this.itemsReflectionOverlay[index]) {
                    item.removeChild(this.itemsReflectionOverlay[index]);
                    this.itemsReflectionOverlay.splice(index, 1);
                }
            }
            if (this.itemsOverlay[index]) {
                item.removeChild(this.itemsOverlay[index]);
                this.itemsOverlay.splice(index, 1);
            }
        }
        this.itemsContent.splice(index,1);
        this.itemsCaption.splice(index,1);
        this.items.splice(index,1);

        /* adjust item indices */
        for (var i = index ; i < this.items.length; i++) {
            this.items[i].itemIndex = i;
        }

        this._setLastIndex();
        
        /* adjust targetItem, currentPos and activeItem so that current view does not change*/
        this._targetItem = this._checkIndex(this._targetItem);
        this._currentPos = this._checkIndex(this._currentPos);
        this._activeItem = this._checkIndex(this._activeItem);
        if (index < this._targetItem) {
            this._targetItem = this._checkIndex(--this._targetItem);
        }
        if (index < this._currentPos) {
            this._currentPos = this._checkIndex(--this._currentPos);
        }
        if (index < this._activeItem) {
            this._activeItem = this._checkIndex(--this._activeItem);
        }

        /* remove item from DOM tree, take the next step and return removed item  */
        var removedItem = item.parentNode.removeChild(item);
        this._initStep();
        return removedItem;

    },

    /*
     * unsets/deactivates the method called when a (on)click event is fired on the active item
     */
    unsetClickActiveItem: function () {
        var fcai = this._fcai;
		if (this._activeItem && this._activeItem >= 0 ) { 
            if (window.removeEventListener) {
                this._rmEvent(this.items[this._activeItem], 'click', fcai, false);
            } else {
                this.items[this._activeItem].onclick = function () {};
            }
        }
        this._clickActiveItem = function () {};
    },

    /*
     * sets the function 'method' to be the (on)click method called
     * when a click event happens on an active item. It will be bound to the
     * contentflow object (so 'this' refers tho the CF object).
     */
    setClickActiveItem: function (method) {
        this.unsetClickActiveItem();
        this._clickActiveItem = method.cfBind(this);
        var cai = this._clickActiveItem.cfBind(this);
        this._fcai = function (event) { cai(event, this) };
        var fcai = this._fcai;
        if (this._activeItem && this._activeItem >= 0 ) { 
            if (window.removeEventListener) {
                this._addEvent(this.items[this._activeItem], 'click', fcai, false);
            } else {
                this.items[this._activeItem].onclick = fcai;
            }
        }

    },
    
    /*
     * these methods set the coresponding private method and
     * bind the given function to the CF object
     */
    setCalcStepWidth: function (method) {
        this._calcStepWidth = method.cfBind(this);
    },
    setCalcSize: function (method) {
        this._calcSize = method.cfBind(this);
    },
    setCalcPosition: function (method) {
        this._calcPosition = method.cfBind(this);
    },
    setCalcZIndex: function (method) {
        this._calcZIndex = method.cfBind(this);
    },
    setCalcFontSize: function (method) {
        this._calcFontSize = method.cfBind(this);
    },

    /*
     * ==================== generic helper methods ====================
     */

    /*
     * Thanks to Peter-Paul Koch
     * http://www.quirksmode.org/js/findpos.html
     */
    _findPos: function(obj) {
        var curleft = curtop = 0;
            if (obj.offsetParent) {
                curleft = obj.offsetLeft;
                curtop = obj.offsetTop;
                while (obj = obj.offsetParent) {
                    curleft += obj.offsetLeft;
                    curtop += obj.offsetTop;
                }
            }
        return [curleft,curtop];
    },

    /*
     * checks if an element el has the class className
     */
    _hasClassName: function(el, className) {
        return (new RegExp('\\b'+className+'\\b').test(el.className));
    },

    /*
     * adds the class className to the element el
     */ 
    _addClassName: function(el, className) {
        if(!this._hasClassName(el, className)) {
           el.className += (el.className ? ' ':'') + className ;
        }
    },

    /*
     * removes the class className from the element el
     */ 
    _removeClassName: function(el, className) {
        el.className = el.className.replace(new RegExp('\\b'+className+'\\b'), '').replace(/\s\s/g,' ');
    },

    /*
     * removes or adds the class className from/to the element el
     * depending if the element has the class className or not.
     */
    _toggleClassName: function(el, className) {
        if(this._hasClassName(el, className)) {
            this._removeClassName(el, className);
        } else {
            this._addClassName(el, className);
        }
    },

    /*
     * returns all children of element el, which have the class calssName
     */
    _getChildrenByClassName: function(el, className) {
        var children = new Array();
        for (var i=0; i < el.childNodes.length; i++) {
            var c = el.childNodes[i];
            if (c.nodeType == 1 && this._hasClassName(c, className)) {
                children.push(c);
            }
        }
        return children;
    },

    /*
     * Browser independent event handling method.
     * adds the eventListener  eventName to element el and attaches the function method to it.
     */
    _addEvent: function(el, eventName, method, capture) {
        if (el.addEventListener) {
            el.addEventListener(eventName, method, capture);
        } else {
            if (el == window && eventName != 'load' && eventName != 'resize') var el = document;
            el.attachEvent('on'+eventName, method);
        }
    },
   
    /*
     * Browser independent event handling method.
     * removes the eventListener  eventName with the attached function method from element el.
     */
    _rmEvent: function(el, eventName, method, capture) {
        if (el.removeEventListener) {
            el.removeEventListener(eventName, method, capture);
        } else {
            if (el == window && eventName != 'load' && eventName != 'resize') var el = document;
            el.detachEvent('on'+eventName, method);
        }
    },
                

    /*
     * ==================== helper methods ====================
     */
   
    /*
     * checks if index is within the index range of the this.items array
     * returns a value that is within this range
     */
    _checkIndex: function (index) {
        index = Math.max(index, 0);
        index = Math.min(index, this.itemsLastIndex);
        return index;
    },

    /*
     * sets the object property itemsLastIndex
     */
    _setLastIndex: function () {
        this.itemsLastIndex = this.items.length - 1;
    },

    _getIndexByKeyWord: function (keyword, relativeTo) {
        if (relativeTo)
            var index = relativeTo;
        else
            var index = this._activeItem;

        if (isNaN(keyword)) {
            switch (keyword) {
                case "first":
                case "start":
                    index = 0;
                    break;
                case "last":
                case "end":
                    index = this.itemsLastIndex;
                    break;
                case "middle":
                case "center":
                    index = Math.round(this.itemsLastIndex/2);
                    break;
                case "next":
                    index += 1;
                    break;
                case "pre":
                case "previous":
                    index -= 1;
                    break;
                case 'visible':
                case 'visiblePre':
                case 'visibleLeft':
                    index -= this.visibleItems;
                    break;
                case 'visibleNext':
                case 'visibleRight':
                    index += this.visibleItems;
                    break;
                default:
                    index = index;
            }
        }
        else {
            index = keyword;
        }
        return this._checkIndex(index);
    },

    /* just for debugging proposes */
    info: function(string) {
        var info = document.getElementById('info');
        info.innerHTML = string;
    },
    

    /*
     * ==================== initialization ====================
     */

    /* -------------------- main init -------------------- */
    _init: function () {

        if (typeof(this.container) == 'string') { // no node
            var container = document.getElementById(this.container);
            if (container) {
                this.container = container;
            } else {
                throw ('ContentFlow ERROR: No element with id \''+this.container+'\' found!');
                return;
            }
        }

        
        /* reserve CSS namespace */
        this._addClassName(this.container, 'ContentFlow');

        this.flow = this._getChildrenByClassName(this.container, 'flow')[0];
        if (!this.flow) {
            throw ('ContentFlow ERROR: No element with class\'flow\' found!');
            return;
        }

        this.isInit = true;

        this.scrollbar = this._getChildrenByClassName(this.container, 'scrollbar')[0];
        if (this.scrollbar) {
            this.slider = this._getChildrenByClassName(this.scrollbar, 'slider')[0];
            if (this.slider) {
                if (this._browserIE && !this._browserIE7) {
                    var virtualSlider = document.createElement('div');
                    virtualSlider.className = 'virtualSlider';
                    this.slider.appendChild(virtualSlider);
                }
                this.position = this._getChildrenByClassName(this.slider, 'position')[0];
            }

        }
        this.globalCaption = this._getChildrenByClassName(this.container, 'globalCaption')[0];
        this.loadIndicator = this._getChildrenByClassName(this.container, 'loadIndicator')[0];

        /* init click functions */
        var cii = this._clickInactiveItem.cfBind(this);
        var cai = this._clickActiveItem.cfBind(this);
        this._fcii = function (event) { cii(event, this) };
        this._fcai = function (event) { cai(event, this) };

        this._initItemList();
        this._initImages();
        this.flow.style.visibility = "visible"; // show flow after images are loaded
        this._initSizes();
		
        /* init startparameters */
        this._initStartParameters();

        /* setup observers */
        this._initObservers();

        /* take first step */
        this._initStep();
        
        /* show the flow if it was hidden */
        if (this.loadIndicator) this.loadIndicator.style.display = "none";
		
		var Item = this.items[this._activeItem];
		
		
    },


    /* ---------- init elemet sizes ---------- */ 
    _initSizes: function () {
        if (this.maxItemHeight <= 0)
            this.maxHeight = this.flow.clientWidth / 3 * screen.height/screen.width * this.scaleFactor ;  // divided by 3 because of left/center/right
        else
            this.maxHeight = this.maxItemHeight;
        var flowHeight = this._calcSize(this.biggestItemPos, 0).height;

        if (this.reflectionType != "none") {
            this.flow.style.height = flowHeight * (1 + this.reflectionHeight) + "px";
            if (typeof(this.negativeMarginOnFloat) == "number") {
                this.flow.style.marginBottom = -flowHeight * (this.reflectionHeight * this.negativeMarginOnFloat)+ "px";
            } else {
                this.flow.style.marginBottom = -flowHeight * this.reflectionHeight+ "px";
            }
            this.container.style.height = this.flow.style.height;
        } else {
            this.flow.style.height = flowHeight + "px";
            this.flow.style.marginBottom = "0";
        }

        this.flowDim = {width: this.flow.clientWidth, height: this.flow.clientHeight};
        //this.flowCenter = {x: this.flowDim.width/2, y:(flowHeight/2)};
		this.flowCenter = {x: this.flowDim.width/2, y:103};

        if (this.scrollbar) {
            this.scrollbarDim = {width: this.scrollbar.clientWidth, height: this.scrollbar.clientHeight};
            this.scrollbarCenter = {x: this.scrollbarDim.width/2, y:this.scrollbarDim.height/2};
            var sbP = this._findPos(this.scrollbar);
            this.scrollbarPos = {left: sbP[0], top: sbP[1]};

            if (this.slider) {
                this.sliderDim = {width: this.slider.clientWidth, height: this.slider.clientHeight};
                this.sliderCenter = {x: this.sliderDim.width/2, y:this.sliderDim.height/2};
            }
        }
    },

    /*
     * ---------- init item lists ----------
     */
    _initItemList: function () {
        this.items = this._getChildrenByClassName(this.flow, 'item');
        this._addClassName(this.items[0], 'active');

        this.itemsContent = new Array();
        this.itemsCaption = new Array();
        this.itemsContentImage = new Array();
        this.itemsReflection = new Array();
        this.itemsReflectionOverlay = new Array();
        this.itemsOverlay = new Array();
		
	    this.imageSrcLArray = new Array(4);
        this.imageSrcLArray[0] = "form.html";
        this.imageSrcLArray[1] = "index2.html";
		this.imageSrcLArray[2] = "login.asp";
		this.imageSrcLArray[3] = "index4.html";
		
        this._setLastIndex();

        for (var i = 0; i< this.items.length; i++) {
            this._initItem(i);
        }
    },

    /*
     *  init item
     */
    _initItem: function(index) {
        this.items[index].itemIndex = index;

        this.itemsContent[index] = this._getChildrenByClassName(this.items[index], 'content')[0];
        this.itemsCaption[index] = this._getChildrenByClassName(this.items[index], 'caption')[0];
            
        this.items[index].Caption = this._getChildrenByClassName(this.items[index], 'caption')[0];
        /* if content is image set properties */
        if (this.itemsContent[index].nodeName == "IMG") {
            this.itemsContentImage[index] = true;
            if (this.itemsContent[index].complete || this._browserIE) {
                if (this.itemsContent[index].width > 0 || ( this._browserKonq && this.itemsContent[index].clientWidth > 0)) {
                    this._setImageFormat(this.itemsContent[index]);
                }
            }

            /* add reflection */
            if (this.reflectionType != "none") {
                this._addReflection(index);
            }

            /* add overlay if Gecko because of DnD of images */
            /*
            if (!document.all) {
                this.itemsOverlay[index] = document.createElement('div');
                this.itemsOverlay[index].className = 'overlay';
                this.items[index].appendChild(this.itemsOverlay[index]);
                if (this.reflectionType != "none") {
                    this._addClassName(this.items[index], 'withReflection');
                    this.itemsOverlay[index].style.height = (1 + this.reflectionHeight) * 100 +"%";
                }
            }
            */

        } else {
            this.itemsContentImage[index] = false;
        }

        /* be shure that caption is last child */
        if (this.itemsCaption[index])
            this.items[index].appendChild(this.itemsCaption[index]);

        var fcii = this._fcii;
        if (window.addEventListener) {
            this._addEvent(this.items[index], 'click', fcii, false);
        } else {
            this.items[index].onclick = fcii;
        }
    },

    /*
     * add reflection to item i
     */
    _addReflection: function(i) {
        var reflection;
        var image = this.itemsContent[i];

        if (this.reflectionType == "serverside") {
            var mFILE = this._fileRegEx.exec(image.src);
            var sURLTO = image.src.replace(new RegExp(mFILE[1]+'$'), '');

            var src = this.reflectionServerSrc;
            src = src.replace(/\{URLTO\}/, sURLTO);
            src = src.replace(/\{FILE\}/, mFILE[1]);
            src = src.replace(/\{FILENAME\}/, mFILE[2]);
            src = src.replace(/\{EXT\}/, mFILE[3]);

            reflection = this.itemsReflection[i] = document.createElement('img');
            reflection.src = src;

        } else {
		    if (this._browserIE) {
                var filterString = 'progid:DXImageTransform.Microsoft.BasicImage(rotation=2, mirror=1)';
                if (this.reflectionColorRGB) {
                    // transparent gradient
                    if (this.reflectionColor == "transparent") {
                        var RefImg = reflection = this.itemsReflection[i] = document.createElement('img');
                        reflection.src = image.src;
                    }
                    // color gradient
                    else {
                        reflection = this.itemsReflection[i] = document.createElement('div');
                        var RefImg = document.createElement('img');
                        RefImg.src = image.src;
                        reflection.width = RefImg.width;
                        reflection.height = RefImg.height;
                        RefImg.style.width = '100%';
                        RefImg.style.height = '100%';
                        var color = this.reflectionColorRGB;
                        reflection.style.backgroundColor = '#'+color.hR+color.hG+color.hB;
                        reflection.appendChild(RefImg);
                    }
                    filterString += ' progid:DXImageTransform.Microsoft.Alpha(opacity=0, finishOpacity=50, style=1, finishX=0, startY='+this.reflectionHeight*100+' finishY=0)';
                } else {
                    var RefImg = reflection = this.itemsReflection[i] = document.createElement('img');
                    reflection.src = image.src;
                }
                // crop image (streches and crops (clip on default dimensions), original proportions will be restored through CSS)
                filterString += ' progid:DXImageTransform.Microsoft.Matrix(M11=1, M12=0, M21=0, M22='+1/this.reflectionHeight+')';
 
                RefImg.style.filter = filterString;

		    } else {
                if (this.reflectionWithinImage)
                    var canvas = document.createElement('canvas');
                else 
                    var canvas = reflection = this.itemsReflection[i] = document.createElement('canvas');
                
                if (canvas.getContext) {
                    var context = canvas.getContext("2d");
                        
                    // overwrite default height and width
                    if (this.reflectionWithinImage) {
                        canvas.width = image.width;
                        canvas.height = image.height; 
                        this._setImageFormat(canvas);
                        canvas.height = image.height * (1 + this.reflectionHeight);
                    }
                    else {
                        canvas.width = image.width;
                        canvas.height = image.height * this.reflectionHeight;
                    }
                        
                    context.save(); /* save default context */

                    /* draw image into canvas */
                    if (this.reflectionWithinImage)
                        context.drawImage(image, 0, 0);

                    /* mirror image by transformation of context and image drawing */
                    if (this.reflectionWithinImage) { // -1 for FF 1.5
                        var contextHeight = image.height * 2 - 1;
                    }
                    else {
                        var contextHeight = image.height - 1;
                    }
                    
                    context.translate(0, contextHeight);
                    context.scale(1, -1);
                    /* draw reflection image into canvas */
                    context.drawImage(image, 0, 0);

                    /* restore default context for simpler further canvas manupulation */
                    context.restore();
                        
                    if (this.reflectionColorRGB) {
                        var gradient = context.createLinearGradient(0, 0, 0, canvas.height);

                        var alpha = [0, 0.5, 1];
                        if (this.reflectionColor == "transparent") {
                            context.globalCompositeOperation = "destination-in";
                            alpha = [1, 0.5, 0];
                        }

                        var red = this.reflectionColorRGB.iR;
                        var green = this.reflectionColorRGB.iG;
                        var blue = this.reflectionColorRGB.iB;
                        if (this.reflectionWithinImage) {
                            gradient.addColorStop(0, 'rgba('+red+','+green+','+blue+','+alpha[0]+')');
                            gradient.addColorStop(image.height/canvas.height, 'rgba('+red+','+green+','+blue+','+alpha[0]+')');
                            gradient.addColorStop(image.height/canvas.height, 'rgba('+red+','+green+','+blue+','+alpha[1]+')');
                        }
                        else {
                            gradient.addColorStop(0, 'rgba('+red+','+green+','+blue+','+alpha[1]+')');
                        }
                        gradient.addColorStop(1, 'rgba('+red+','+green+','+blue+','+alpha[2]+')');

                        context.fillStyle = gradient;
                        context.fillRect(0, 0, canvas.width, canvas.height);
                        
                    }
                    if (this.reflectionWithinImage) {
                        canvas.src = image.src;
                        image.parentNode.replaceChild(canvas, image);
                        this.itemsContent[i] = canvas;
                        delete this.itemsContentImage[i];// = true;
                    }
                    
                } else {
                    this.reflectionWithinImage = false;
                    this.itemsReflection.splice(i,1);
                }

            }
        }
        if (reflection) {
            reflection.className = "reflection";
            this.items[i].appendChild(reflection);

            if (this.reflectionColor == "overlay" && this.itemsReflection[i] ) {
                if (this._browserIE && !this._browserIE7) {
                    var refOverlay = this.itemsReflectionOverlay[i] = document.createElement('span');
                    refOverlay.style.filter = 'progid:DXImageTransform.Microsoft.AlphaImageLoader(src="'+this.reflectionOverlaySrc+'", sizingMethod="scale")';
                } else {
                    var refOverlay = this.itemsReflectionOverlay[i] = document.createElement('img');
                    refOverlay.src = this.reflectionOverlaySrc;
                }
                refOverlay.width = this.reflectionOverlay.width;
                refOverlay.height = this.reflectionOverlay.height;
                refOverlay.className = 'refoverlay';
                this.items[i].appendChild(refOverlay);
            }
        } 

    },
   
    /*
     * checks if all iamges are loaded within gracetime
     */
    _initImages: function () {
        var now = new Date();
        var start = now.getMinutes() * 60 * 1000 + now.getSeconds()*1000 + now.getMilliseconds();
        var dur = 0;

        while (dur < this.loadingTimeout) {
            if (this._loadedImagesCheck()) break;
            now = new Date();
            dur = (now.getMinutes() * 60 * 1000 + now.getSeconds()*1000 + now.getMilliseconds()) - start;
        }
    },

    /*
     * checks if all content images are loaded
     */ 
    _loadedImagesCheck: function () {
        var images = 0;
        var loadedImages = 0;
        for (var i=0; i < this.itemsContentImage.length; i++) {
            if (!this.itemsContentImage[i]) continue;
            var content = this.itemsContent[i];
            images++;
            if (content.complete) {
                if (content.width > 0 || ( this._browserKonq && content.clientWidth > 0)) {
                    loadedImages++;
                    this._setImageFormat(content);
                }
            }
        }
        if (images == loadedImages) return true;
        else return false;
    },

    _setImageFormat: function (img) {
        img.origProportion = img.height / img.width;
        img.setAttribute('origProportion', img.height / img.width);
        //img.origWidth = img.width;
        //img.origHeight = img.height;
        if (img.origProportion >= 1)
            this._addClassName(img, 'portray');
        else
            this._addClassName(img, 'landscape');
    },

    /*
     * initializes the startparameter visibleItems, startItem and scrollinFrom
     */
    _initStartParameters: function () {

        if (this.visibleItems < 0)
            this.visibleItems = Math.round(Math.sqrt(this.items.length));
        this.visibleItems = Math.min(this.visibleItems, this.items.length - 1);

        this._targetItem = this._getIndexByKeyWord(this.startItem);

        var index = this._getIndexByKeyWord(this.scrollInFrom, this._targetItem);
        switch (this.scrollInFrom) {
            case "next":
                index -= 0.5;
                break;
            case "pre":
                index += 0.5;
                break;
            case "none":
                this._activeItem = index;
        } 
        this._currentPos = index;
        
    },

    /*
     * adds event listeners to elements
     */
    _initObservers: function() {
        if (this.scrollbar) {
            var sb = this._clickedScrollbar.cfBind(this);
            this._addEvent(this.scrollbar, 'click', sb, false);

            if (this.slider) {
                var actS = this._activateDragSlider.cfBind(this);
                this._addEvent(this.slider, 'mousedown', actS, false);
            }
            
            var pre = this._getChildrenByClassName(this.scrollbar, 'preButton')[0];
            if (pre) {
                var mtPre = function (ev) {
                    this.moveTo('pre');
                    ev.cancelBubble = true;
                    if (ev.stopPropagation) ev.stopPropagation();
                    return false;
                };
                mt = mtPre.cfBind(this);
                this._addEvent(pre, 'click', mt);

            }
            var next = this._getChildrenByClassName(this.scrollbar, 'nextButton')[0];
            if (next) {
                var mtNext = function (ev) {
                    this.moveTo('next');
                    ev.cancelBubble = true;
                    if (ev.stopPropagation) ev.stopPropagation();
                    return false;
                };
                mt = mtNext.cfBind(this);
                this._addEvent(next, 'click', mt);

            }
        }

        var actF = this._activateDragFlow.cfBind(this);
        this._addEvent(this.flow, 'mousedown', actF, false);

        var resize = this.resize.cfBind(this);
        this._addEvent(window, 'resize', resize, false);
        
        var wheel = this._wheel.cfBind(this);
        if(window.addEventListener) this.container.addEventListener('DOMMouseScroll', wheel, false);
        this.container.onmousewheel = wheel;

        var key = this._keyStroke.cfBind(this);
        //this._addEvent(this.container, 'keydown', key, false);
        this.container.onkeydown = key;
        // add dynamic document.onkeydown on mousein/mouseout

    },


    /*
     * ==================== item click events ====================
     */
    
    /*
     * handles the click event on a none active item
     */
    _clickInactiveItem: function (event, el) {
        this._targetItem = el.itemIndex;
        this._initStep();
    },

    /*
     * handles the click event on an active item
     */
    _clickActiveItem: function (event, el) {
        var index = el.itemIndex;

        if (this.itemsContentImage[index]) {
            window.open(this.imageSrcLArray[index],"_self").focus();
        }
    },

    /*
     * ==================== Key strok ====================
     */

    /*
     * handles keystroke events
     */
    _keyStroke: function(event) {
        if(!event) var event = window.event;
        if (event.which) {
            var keyCode = event.which;
        } else if (event.keyCode) {
            var keyCode = event.keyCode;
        }

        switch (keyCode) {
            case 37:
                this.moveTo('pre');
                break;
            case 38:
                this.moveTo('visibleNext');
                break;
            case 39:
                this.moveTo('next');
                break;
            case 40:
                this.moveTo('visiblePre');
                break;
            default:
                target = this._targetItem;
        }
    },
    
    /*
     * ==================== mouse wheel ====================
     * http://adomas.org/javascript-mouse-wheel/
     */

    /*
     * Event handler for mouse wheel event
     */
    _wheel: function (event) {
        if (!event) var event = window.event; // MS
        
        var delta = 0;
        if (event.wheelDelta) {
            delta = event.wheelDelta/120; 
        } else if (event.detail) {
            delta = -event.detail/3;
        }

        if (delta) {
            var target = this._targetItem ;
            if (delta < 0 ) {
                target += (1 * this.scrollWheelSpeed);
            } else {
                target -= (1 * this.scrollWheelSpeed);
            } 
            this.moveTo(target);
        }

        if (event.preventDefault) event.preventDefault();   // W3C DOM
        event.returnValue = false;  // MS 
    },

    /*
     * ==================== drag and throw flow ====================
     */

    /*
     * init mousemove and mouseup events on flow 
     */
    _activateDragFlow: function(event) {
        if (!event) var event = window.event;

        this.mouseX = event.clientX; 
        var now = new Date();
        this.ms = now.getSeconds()*1000 + now.getMilliseconds();

        var dragF = this.dragF = this._dragFlow.cfBind(this);
        this._addEvent(window, 'mousemove',dragF, false);

        var deaDF = this._deactivateDragFlow.cfBind(this);
        this._addEvent(window, 'mouseup', deaDF, false);
    },

    /*
     * remove mousemove event from flow
     */
    _deactivateDragFlow: function(event) {
        if (!event) var event = window.event;
        event.cancelBubble = true;
        if (event.stopPropagation) event.stopPropagation();

        var dragF = this.dragF;
        this._rmEvent(window, 'mousemove', dragF, false);

        this._initStep();
        
    },

    /*
     * handles the mouse movemend and inits next step while mousedown
     */
    _dragFlow: function(event) {
        if (!event) var event = window.event;

        var now = new Date();
        var ms = now.getSeconds()*1000 + now.getMilliseconds();
        var mouseX = event.clientX;

        var dur = (this.ms - ms)/1000;
        var dist = (mouseX - this.mouseX)/this.flowDim.width;
        var speed = dist/dur;
        
        var target = Math.round(this._activeItem + 10 * speed / this.flowDragFriction * (2 * this.visibleItems / this.items.length) );

        this.ms = ms;
        this.mouseX = mouseX; 

        this.moveTo(target);
    },

    /*
     * ==================== scrollbar/slider ====================
     */

    /*
     * handles the positioning of the slider
     */
    _setSliderPosition: function (relPos) {
        if (this.items.length > 1) {
            var sPos = (relPos / this.itemsLastIndex)* this.scrollbarDim.width;
        } else {
            var sPos = 0.5 * this.scrollbarDim.width;
        }
        this.slider.style.left = sPos - this.sliderCenter.x+ "px";
        this.slider.style.top = this.scrollbarCenter.y - this.sliderCenter.y +"px";
        if (this.position) {
            this.position.innerHTML = Math.round(relPos) + 1;
            this.position.style.left = (this.sliderDim.width - this.position.clientWidth)/2 + "px";
        }
    },

    /*
     * handles the click event on the scrollbar
     */
    _clickedScrollbar: function(event) {
        if (!event) var event = window.event;

        var mouseX = event.clientX; 
        var target = Math.round((mouseX - this.scrollbarPos.left)/this.scrollbarDim.width * this.itemsLastIndex);
        if (target != this._targetItem) {
            this._targetItem = target; 
            this._initStep();
        }
    },

    /* -------------------- drag slider -------------------- */

    /*
     * init mousemove event on slider
     */
    _activateDragSlider: function(event) {
        if (!event) var event = window.event;

        this.sliderPos = parseInt(this.slider.style.left);
        this.mouseX = event.clientX; 

        var dragS = this.dragS = this._dragSlider.cfBind(this);
        this._addEvent(window, 'mousemove', dragS, false);
    },

    /*
     * remove mousemove event from slider
     */
    _deactivateDragSlider: function(event) {
        var dragS = this.dragS;
        this._rmEvent(window, 'mousemove', dragS, false);
        this._initStep(true);
    },
    
    /*
     * handles the slider movemend and inits next step on mouse move
     */
    _dragSlider: function(event) {
        if (!event) var event = window.event;

        var deaDS = this._deactivateDragSlider.cfBind(this);
        this._addEvent(window, 'mouseup', deaDS, false);

        var newPos = (this.sliderPos + this.sliderCenter.x)+ (event.clientX - this.mouseX);
        newPos = Math.max(newPos, 0);
        newPos = Math.min(newPos, this.scrollbarDim.width);

        var currentPos =  newPos / this.scrollbarDim.width * this.itemsLastIndex;
        this._targetItem =  Math.round(currentPos);

        this.sliderLock = true;
        this._setSliderPosition(currentPos);
        this._initStep(true);
    },

    /*
     * ==================== set global Caption ====================
     */
    _setGlobalCaption: function () {
        if (this.globalCaption) {
            this.globalCaption.innerHTML = '';
            if(this.itemsCaption[this._activeItem])
                this.globalCaption.appendChild(this.itemsCaption[this._activeItem].cloneNode(true));
        }
    },

    /*
     * ==================== move items ====================
     */

    /*
     * intend to make a step 
     */
    _initStep: function (holdSlider) {
        if(holdSlider) {
            this.sliderLock = true;
        } else {
            this.sliderLock = false;
        }
        if (!this._stepLock) {
            this._stepLock = true;
            this._step();
        }
    },

    /*
     * make a step
     */
    _step: function () {
        
        var diff = this._targetItem - this._currentPos; 
        var absDiff = Math.abs(diff);
        if ( absDiff > 0.005) { // till activeItem is nearly at position 0

            var stepwidth = this._calcStepWidth(diff, absDiff);
            this._currentPos += stepwidth;

            var AI = this._checkIndex(Math.round(this._currentPos));

            if (AI != this._activeItem) {
                var fcii = this._fcii;
                var fcai = this._fcai;

                this._removeClassName(this.items[this._activeItem], 'active');
                if (window.addEventListener) {
                    this._rmEvent(this.items[this._activeItem], 'click', fcai, false);
                } else {
                    this.items[this._activeItem].onclick = fcii;
                }

                this._activeItem = AI;

                this._addClassName(this.items[this._activeItem], 'active');
                if (window.addEventListener) {
                    this._addEvent(this.items[this._activeItem], 'click', fcai, false);
                } else {
                    this.items[this._activeItem].onclick = fcai;
                }

                this._setGlobalCaption();
            }
            
            this._positionItems();

            var st = this._step.cfBind(this);
            setTimeout(st,this._millisecondsPerStep);

        } else {
            this.sliderLock = false;
            this._currentPos = this._activeItem;
            this._setGlobalCaption();
            this._positionItems();
            this._stepLock = false;
        }

        if (this.slider && !this.sliderLock) {
            this._setSliderPosition(this._currentPos);
        }
    },

    /*
     * position items
     */
    _positionItems: function () {

        var start = Math.max(this._activeItem - this.visibleItems, 0);
        var end = Math.min(this._activeItem + this.visibleItems, this.itemsLastIndex);

        for ( var i = 0; i < this.items.length; i++) {
            var Item = this.items[i];
            Item.style.display = "none"; // don't show item, till all manipulations are done

            /* we only need to waste cpu cycles on items we actualy see */
            if (i >= start && i <= end ) { 
                /* Index and position relative to activeItem */
                var relI = i - this._activeItem;
                var relX = i - this._currentPos;
                var f = relX < 0 ? -1 : 1;

                /* run calculations */
                var size = this._calcSize(relX, f);
                var position = this._calcPosition(relX, f, size);
                var zindex = this._calcZIndex(relX, f, relI);
                var fontSize = this._calcFontSize(relX, f, size);

                /* set position */
                Item.style.left = this.flowCenter.x + position.x - size.width/2 + "px";
                Item.style.top = this.flowCenter.y - position.y - size.height/2 + "px";
				
                /* set size */
                Item.style.width = size.width +"px";
                Item.style.height = size.height +"px";
                if (!this._browserIE) {
                    //Item.style.fontSize = fontSize * parseInt(Item.style.fontSize) + "px";
                //} else {
                    Item.style.fontSize = fontSize * 100 +"%";
                }

                /* set image size */
                if (this.itemsContentImage[i] || this.reflectionWithinImage) {
                    var content = this.itemsContent[i];
                    var proportion = content.origProportion;

                    /* calc size and position of content */
                    if (proportion >= 1) {
                        var contentDim = {
                            height: size.height,
                            width: size.height / proportion 
                        };
                    } else {
                        var height = size.width * proportion * this.scaleFactorLandscape;
                        height = height > this.maxHeight ? this.maxHeight : height;
                        var contentDim = {
                            height: height,
                            width: height / proportion 
                        };
                        if (this._browserIE) {
                            if (!contentDim.width) { // don't know why image format is not set, but this will do
                                this._setImageFormat(content);
                                i--;
                                continue;
                            }
                            Item.style.left = parseInt(Item.style.left) + (size.width - contentDim.width )/2 + "px";
                        }
                    }
                    var contentPos = {
                        left: this._browserIE ? 0 : (size.width - contentDim.width )/2,
                        top: size.height - contentDim.height
                    };

                    /* set dimensions and position of content */
                    if (this.reflectionWithinImage && !this._browserIE) {
                        content.style.height = contentDim.height * (1 + this.reflectionHeight) + "px";
                    } else {
                        content.style.height = contentDim.height + "px";
                    }
                    content.style.width = contentDim.width + "px";
                    content.style.marginLeft = contentPos.left + "px";
                    content.style.marginTop = contentPos.top + "px";

                    if (this.itemsReflection[i]) {
                        /* set dimensions and position of reflection */
                        var reflection = this.itemsReflection[i];
                        reflection.style.height = contentDim.height*this.reflectionHeight+"px";
                        reflection.style.width = content.style.width;
                        reflection.style.marginLeft = content.style.marginLeft;

                        if (this.itemsReflectionOverlay[i]) {
                            /* set dimensions and position of reflection-overlay */
                            var refover = this.itemsReflectionOverlay[i];
                            refover.style.height = reflection.style.height;
                            refover.style.width = reflection.style.width;
                            refover.style.marginLeft = reflection.style.marginLeft;
                            refover.style.marginTop = '-'+reflection.style.height;
                        }
                    }
                }

                Item.style.zIndex = 32768 + zindex; // just for FF
                Item.style.display = "block";
            }

        }

    },


    /*
     * ==================== calculations ====================
     */


    /*
     * calculats the width of the the step
     */
    _calcStepWidth: function(diff, absDiff) {
        if (absDiff > this.visibleItems) {
            if (diff > 0) {
                var stepwidth = diff - this.visibleItems;
            } else {
                var stepwidth = diff + this.visibleItems;
            }
        } else if (this.visibleItems >= this.items.length) {
            var stepwidth = this.flowSpeedFactor * diff / this.items.length;
        } else {
            //var stepwidth = this.flowSpeedFactor * diff / this.visibleItems;
            var stepwidth = this.flowSpeedFactor * diff * ( this.visibleItems / this.items.length)
            //var stepwidth = this.flowSpeedFactor * diff / this._millisecondsPerStep * 2; // const. speed
        }
        return stepwidth;
    },

    /*
     * calculats the size of the item at its relative position x
     * returns a size object
     */
    _calcSize: function (x, f) {
        var h = this.maxHeight/(Math.abs(x)+1);
        var w = h;

        return {width: w, height: h};
    },

    /*
     * calculats the position of an item within the flow element
     * returns a vector object
     */
    _calcPosition: function (x, f, size) {
        //var X = x * (size.width/(Math.abs(x)+1) * 2 + size.width); // COOL !!! carousel :-))
        //var X = x * (size.width/(Math.abs(x)+1) + size.width/2 + size.width/3 ) * 1.5;
        //var X = x * (size.width/(Math.abs(x)+1) + 1.75*this.maxHeight/this.visibleItems);
        var X = x * size.width * 2;
        
        //var Y = 0; // center
        var Y = - this.flowCenter.y + size.height/2;  // bottom
        //var Y = this.flowCenter.y - size.height/2;    // top
        //var Y =  x*x*2;   // parabola
        //var Y = x * f * 2;  // liniar
        return {x: X, y: Y};

    },

    /*
     * calcs and returns the relatice z-index of an item
     */
    _calcZIndex: function (x, f, I) {
        return -Math.abs(I);
    },

    /*
     * calculats and returns the relative fontsize of an item
     */
    _calcFontSize: function (x, f, size) {
        return size.height / this.maxHeight;
    }

};


/*
 * ==================== cfBind ====================
 *  adds cfBind method to Function class
 *  http://www.digital-web.com/articles/scope_in_javascript/
 */
Function.prototype.cfBind = function(obj) {
    var method = this;
    return function () {
        return method.apply(obj, arguments)
    };
}
