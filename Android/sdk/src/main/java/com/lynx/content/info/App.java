// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content.info;

import java.util.ArrayList;

public class App {
    String packageName;
    String icon;
    String name;
    String versionCode;
    String versionName;
    String mainPage;
    ArrayList<Page> pages;

    public String getPackageName() {
        return packageName;
    }

    public void setPackageName(String packageName) {
        this.packageName = packageName;
    }

    public String getIcon() {
        return icon;
    }

    public void setIcon(String icon) {
        this.icon = icon;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getVersionCode() {
        return versionCode;
    }

    public void setVersionCode(String versionCode) {
        this.versionCode = versionCode;
    }

    public String getVersionName() {
        return versionName;
    }

    public void setVersionName(String versionName) {
        this.versionName = versionName;
    }

    public String getMainPage() {
        return mainPage;
    }

    public void setMainPage(String mainPage) {
        this.mainPage = mainPage;
    }

    public ArrayList<Page> getPages() {
        return pages;
    }

    public void setPages(ArrayList<Page> pages) {
        this.pages = pages;
    }

    public Page getPage(String name) {
        for (int i = 0; i < pages.size(); i++) {
            Page page = pages.get(i);
            if (page.getName().equals(name)
                    || page.getPageName().equals(name)) {
                return page;
            }
        }
        return null;
    }

    public boolean isLegalPage(String name) {
        if (getPage(name) != null) {
            return true;
        }
        return false;
    }

    public boolean isLegalPage(Page page) {
        return isLegalPage(page.getPageName());
    }

}
