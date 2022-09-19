

Testing basic method of "lazy evaluation" or "evaluation on read" of properties.  This moves calculations to the GUI thread however, potentially slowing it down, especially if calcualation is longer than a few ms.  

The goal is to avoid unneccesary computation of data which won't be displayed, or only calculate data once which is used in multiple places in the GUI. 

Data is provided by DataItem objects (QObjects with properties that could be members of a data model), each of which calculates its data value based on one DataSource's input data value, which is expected to change periodically or spuriously at any time. 

Other ideas are 
  * only calculate data during update if included in a certain data model or if a certain QML element is enabled (has true visible property).
  * Use a Loader in GUI views, which puts a request in a worker thread (in QML Javascript or in C++) that does the read asynchronously from GUI display.  Check DataItem first to know whether we should do this (because recalculation is needed) or to just display a normal element or delegate in the view with a direct read (because we'll be getting the cached precalculated data).
