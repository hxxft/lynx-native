import Vue from 'vue' 
               import App from '../src/app.vue' 
                export function createApp () { 
                const app = new Vue({ 
                render: h => h(App) 
                }) 
                return { app } 
                } 
