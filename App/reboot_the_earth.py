#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
from datetime import datetime
import numpy as np
date_rng = pd.date_range(start='1/1/2019', end='31/12/2019', freq='D')
np.random.seed(1)


# In[2]:


df = pd.DataFrame(date_rng, columns=['date'])
No_of_ypos=150
No_of_xpos=5
days_of_travel=30 # Assuming the days of travel to be 30 and ship will cross 1 ypos in 1 day


# In[3]:


A0=np.random.randint(100, size=(len(date_rng), No_of_ypos*No_of_xpos)) #Sensor data from nodes
#B=np.ones_like(A) 
B=np.random.randint(2,size=(len(date_rng), No_of_ypos*No_of_xpos)) # datasets from fisheries about fish reproduction region
#Creating a rejection criterion
for i in range(B.shape[0]):
    for j in range(B.shape[1]):
        if B[i,j]==0:
            B[i,j]=10
A=A0*B


# In[4]:


x=pd.DataFrame(A)
y=pd.DataFrame(A0)


# In[5]:


data=pd.concat([df, x],axis=1)
data_wo=pd.concat([df, y],axis=1)


# In[6]:


data.head()


# In[7]:


#finding the optimum path
pos=np.zeros(days_of_travel)
col=1;
sol=500;
for i in range(days_of_travel):
    for j in range(No_of_xpos):
        y=data.iloc[i,col+j]
        if y<sol:
            sol=y
            pos[i]=col+j
        else:
            sol=sol
    col=col+No_of_xpos
    sol=500
    


# In[8]:


pos


# In[10]:


#data visualization
#creating the relevant matrix data
col=1;
picture0=np.zeros((days_of_travel,No_of_xpos))
for i in range(days_of_travel):
    for j in range(No_of_xpos):
        picture0[i,j]=data.iloc[i,col+j]
    col=col+No_of_xpos
#creating the relevant matrix data_wo
col=1;
picture1=np.zeros((days_of_travel,No_of_xpos))
for i in range(days_of_travel):
    for j in range(No_of_xpos):
        picture1[i,j]=data_wo.iloc[i,col+j]
    col=col+No_of_xpos


# In[12]:


picture0


# In[15]:


import matplotlib.pyplot as plt
fig=plt.figure(figsize=(30,30))
plt.imshow(picture1)
plt.colorbar()
plt.show()


# In[14]:


fig=plt.figure(figsize=(30,30))
plt.imshow(picture0)
plt.colorbar()
plt.show()

