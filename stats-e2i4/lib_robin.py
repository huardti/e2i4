import numpy as np
import pandas as pd
from scipy.stats import norm, t, ttest_1samp, ttest_ind, shapiro, bartlett
from scipy.stats.stats import f_oneway
import matplotlib.pyplot as plt

################################################################################

def normal_law_value(Nc: float, bilateral: bool) -> float:
    ''' Calculates the Normal law value associated with confidence level Nc '''

    # If we need to consider alpha/2
    if bilateral:
        # Return the normal law value associated with probability
        # P = Nc + alpha/2 = Nc + (1 - Nc)/2 = (Nc + 1)/2
        return norm.ppf((Nc + 1) /2)
    # If we need to consider alpha
    else:
        # P = Nc
        return norm.ppf(Nc)

################################################################################

def student_law_value(Nc: float, size: int, bilateral: bool) -> float:
    ''' Calculates the Student law value associated with confidence level Nc '''

    if bilateral:
        return t.ppf((Nc + 1) /2, size - 1)
    else:
        return t.ppf(Nc, size - 1)

################################################################################

def mean_confidence_interval(   sample: np.ndarray,
                                confidence_level: float,
                                real_var: float = 0
                                ) -> np.ndarray(2, float):
    '''
    Calculates the mean confidence interval associated with a sample, with a
    specified confidence_level
    '''

    sample_mean = sample.mean()

    if(real_var <= 0):
        variance = sample.var(ddof = 1)
    else :
        variance = real_var

    if sample.size < 30:
        z = student_law_value(confidence_level, sample.size, bilateral=True)
    else:
        z = normal_law_value(confidence_level, bilateral=True)

    y = z * np.sqrt(variance / sample.size)

    return [ sample_mean - y, sample_mean + y ]

################################################################################

def proportion_confidence_interval( sample: np.ndarray,
                                    confidence_level: float
                                    ) -> np.ndarray(2, float):
    '''
    Calculates the proportion confidence interval associated with a sample, with
    a specified confidence_level
    '''

    f = sample.mean()
    z = normal_law_value(confidence_level, bilateral=True)

    y = z * np.sqrt( (f*(1 - f)) / sample.size)

    return [ f - y, f + y ]

################################################################################

class test_hypothese_1samp:

    def __init__(self,
                    sample: pd.Series,
                    mean: float,
                    bilateral: bool,
                    confidence_level: float = 0.95  ):

        if shapiro(sample).pvalue < 0.05:
            print("ERROR : Sample failed the Shapiro test")
            return

        self.alpha = 1 - confidence_level
        self.bilateral = bilateral
        self.z_obs = ttest_1samp(sample, mean)
        self.z_alpha = normal_law_value(confidence_level, bilateral=bilateral)

    def test(self) -> bool:
        ''' Returns true if the null hypothesis is rejected, false otherwise '''

        return self.z_obs.pvalue < self.alpha

    def __bool__(self) -> bool:
        return bool(self.test())

    def __repr__(self) -> str:
        if self:
            return "The null hypothesis is rejected"
        else:
            return "The null hypothesis is NOT rejected"

################################################################################

class test_hypothese_ind:

    def __init__(self,
                    sample1: pd.Series,
                    sample2: pd.Series,
                    bilateral: bool,
                    confidence_level: float = 0.95  ):

        if shapiro(sample1).pvalue < 0.05:
            print("ERROR : Sample 1 failed the Shapiro test")
            return
        elif shapiro(sample2).pvalue < 0.05:
            print("ERROR : Sample 2 failed the Shapiro test")
            return
        elif bartlett(sample1, sample2).pvalue < 0.05:
            print("ERROR : Samples' variances are not homogeneous")
            return

        self.alpha = 1 - confidence_level
        self.bilateral = bilateral
        self.z_obs = ttest_ind(sample1, sample2, equal_var=False)
        self.z_alpha = normal_law_value(confidence_level, bilateral=bilateral)

    def test(self) -> bool:
        ''' Returns true if the null hypothesis is rejected, false otherwise '''

        return self.z_obs.pvalue < self.alpha

    def __bool__(self) -> bool:
        return bool(self.test())

    def __repr__(self) -> str:
        if self:
            return "The null hypothesis is rejected"
        else:
            return "The null hypothesis is NOT rejected"

################################################################################

class test_hypothese_fisher:

    def __init__(self,
                    *samples: pd.Series,
                    confidence_level: float = 0.95  ):

        i = 1
        for sample in samples:
            if shapiro(sample).pvalue < 0.05:
                print("ERROR : Sample {} failed the Shapiro test" % i)
                return
            i += 1

        if bartlett(*samples).pvalue < 0.05:
            print("ERROR : Samples' variances are not homogeneous")
            return

        self.alpha = 1 - confidence_level
        self.f_obs = f_oneway(*samples)

    def test(self) -> bool:
        ''' Returns true if the null hypothesis is rejected, false otherwise '''

        return self.f_obs.pvalue < self.alpha

    def __bool__(self) -> bool:
        return bool(self.test())

    def __repr__(self) -> str:
        if self:
            return "The null hypothesis is rejected : at least one mean is " \
                    "different"
        else:
            return "The null hypothesis is NOT rejected : all means are "\
                    "identical"

################################################################################

def plot_bar(sample: pd.Series, xlabel: str, ylabel: str, axis: plt.Axes=None) -> plt.Axes:

    axis = axis or plt.gca()

    pd.Series(sample).value_counts().sort_index().plot.bar(ax=axis, x=0, width=0.1)

    axis.set_xlabel(xlabel)
    axis.set_ylabel(ylabel)

    return axis

################################################################################

def plot_hist(sample: pd.Series, xlabel: str, ylabel: str, axis: plt.Axes=None) -> plt.Axes:

    axis = axis or plt.gca()

    pd.Series(sample).plot.hist(
        ax=axis,
        density=True,
        bins=int(np.sqrt(len(sample)))
    )

    axis.set_xlabel(xlabel)
    axis.set_ylabel(ylabel)

    return axis

################################################################################

def plot_box(sample: pd.Series, xlabel: str, ylabel: str, axis: plt.Axes=None) -> plt.Axes:

    axis = axis or plt.gca()

    pd.Series(sample).plot.box(ax=axis, vert=False)

    axis.set_xlabel(xlabel)
    axis.set_ylabel(ylabel)

    return axis

################################################################################

def show_hist_box(sample: pd.Series, title: str, xlabel: str):

    fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)

    plot_hist(sample, xlabel, "Relative frequency", ax1)
    plot_box(sample, xlabel, " ", ax2)

    ax1.tick_params(reset=True)
    ax2.set_yticks([])

    fig.suptitle(title)

    plt.show()
