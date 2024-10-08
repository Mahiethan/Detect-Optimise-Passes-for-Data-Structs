; ModuleID = 'static_AoS.c'
source_filename = "static_AoS.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeTwo = type { float, double }
%struct.nodeOne = type { i32, i8, double }

@recursiveCount = dso_local global i32 0, align 4
@globalFour = dso_local global [100 x %struct.nodeTwo] zeroinitializer, align 16
@globalFive = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalThree = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalOne = dso_local global [100 x %struct.nodeOne] zeroinitializer, align 16
@globalTwo = dso_local global [100 x %struct.nodeTwo] zeroinitializer, align 16
@invalidGlobal = dso_local global %struct.nodeOne zeroinitializer, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOne(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOne, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 22, ptr %a, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeTwo(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 1.200000e+01, ptr %a, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateThree() #0 {
entry:
  %f = alloca i32, align 4
  %saved_stack = alloca ptr, align 8
  %__vla_expr0 = alloca i64, align 8
  %recursive = alloca ptr, align 8
  store i32 100, ptr %f, align 4
  %0 = load i32, ptr %f, align 4
  %1 = zext i32 %0 to i64
  %2 = call ptr @llvm.stacksave()
  store ptr %2, ptr %saved_stack, align 8
  %vla = alloca %struct.nodeTwo, i64 %1, align 16
  store i64 %1, ptr %__vla_expr0, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %vla, i64 67
  %b = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 1
  store double 9.023000e+01, ptr %b, align 8
  %call = call noalias ptr @calloc(i64 noundef 100, i64 noundef 16) #3
  store ptr %call, ptr %recursive, align 8
  %3 = load i32, ptr @recursiveCount, align 4
  %cmp = icmp ne i32 %3, 5
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %4 = load i32, ptr @recursiveCount, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr @recursiveCount, align 4
  %5 = load ptr, ptr %recursive, align 8
  call void @populateTwo(ptr noundef %5)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %6 = load ptr, ptr %saved_stack, align 8
  call void @llvm.stackrestore(ptr %6)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare ptr @llvm.stacksave() #1

; Function Attrs: nounwind allocsize(0,1)
declare dso_local noalias ptr @calloc(i64 noundef, i64 noundef) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateTwo(ptr noundef %ptr) #0 {
entry:
  %ptr.addr = alloca ptr, align 8
  store ptr %ptr, ptr %ptr.addr, align 8
  %0 = load ptr, ptr %ptr.addr, align 8
  %cmp = icmp ne ptr %0, null
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load ptr, ptr %ptr.addr, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %1, i64 100
  %a = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 7.000000e+00, ptr %a, align 8
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  call void @populateThree()
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.stackrestore(ptr) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populate() #0 {
entry:
  %f = alloca i32, align 4
  %saved_stack = alloca ptr, align 8
  %__vla_expr0 = alloca i64, align 8
  store i32 100, ptr %f, align 4
  %0 = load i32, ptr %f, align 4
  %1 = zext i32 %0 to i64
  %2 = call ptr @llvm.stacksave()
  store ptr %2, ptr %saved_stack, align 8
  %vla = alloca %struct.nodeOne, i64 %1, align 16
  store i64 %1, ptr %__vla_expr0, align 8
  call void @populateTwo(ptr noundef null)
  %3 = load ptr, ptr %saved_stack, align 8
  call void @llvm.stackrestore(ptr %3)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam0(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam1(ptr noundef %0, i32 noundef %1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam1(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load i32, ptr %n.addr, align 4
  %cmp = icmp sgt i32 %0, 100
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, ptr %n.addr, align 4
  %dec = add nsw i32 %1, -1
  store i32 %dec, ptr %n.addr, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %n.addr, align 4
  call void @populateParam2(ptr noundef %2, i32 noundef %3)
  store float 9.900000e+01, ptr getelementptr inbounds ([100 x %struct.nodeTwo], ptr @globalFour, i64 0, i64 99), align 16
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam2(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %innerArrayOne = alloca [100 x %struct.nodeOne], align 16
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam3(ptr noundef %0, i32 noundef %1)
  %arrayidx = getelementptr inbounds [100 x %struct.nodeOne], ptr %innerArrayOne, i64 0, i64 0
  %a = getelementptr inbounds %struct.nodeOne, ptr %arrayidx, i32 0, i32 0
  store i32 99, ptr %a, align 16
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam3(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 100, ptr getelementptr inbounds ([100 x %struct.nodeOne], ptr @globalFive, i64 0, i64 1), align 16
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateParam4(ptr noundef %0, i32 noundef %1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateParam4(ptr noundef %array, i32 noundef %n) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  %0 = load ptr, ptr %array.addr, align 8
  %1 = load i32, ptr %n.addr, align 4
  call void @populateNodeOne(ptr noundef %0, i32 noundef %1)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %saved_stack = alloca ptr, align 8
  %__vla_expr0 = alloca i64, align 8
  %b = alloca i32, align 4
  %__vla_expr1 = alloca i64, align 8
  %c = alloca i32, align 4
  %__vla_expr2 = alloca i64, align 8
  %e = alloca i32, align 4
  %__vla_expr3 = alloca i64, align 8
  store i32 0, ptr %retval, align 4
  store i32 100, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = zext i32 %0 to i64
  %2 = call ptr @llvm.stacksave()
  store ptr %2, ptr %saved_stack, align 8
  %vla = alloca %struct.nodeOne, i64 %1, align 16
  store i64 %1, ptr %__vla_expr0, align 8
  %3 = load i32, ptr %a, align 4
  call void @populateNodeOne(ptr noundef %vla, i32 noundef %3)
  store i32 100, ptr %b, align 4
  %4 = load i32, ptr %b, align 4
  %5 = zext i32 %4 to i64
  %vla1 = alloca %struct.nodeTwo, i64 %5, align 16
  store i64 %5, ptr %__vla_expr1, align 8
  %arrayidx = getelementptr inbounds %struct.nodeTwo, ptr %vla1, i64 23
  %a2 = getelementptr inbounds %struct.nodeTwo, ptr %arrayidx, i32 0, i32 0
  store float 0x3FF051EB80000000, ptr %a2, align 16
  store i32 100, ptr %c, align 4
  %6 = load i32, ptr %c, align 4
  %7 = zext i32 %6 to i64
  %vla3 = alloca i32, i64 %7, align 16
  store i64 %7, ptr %__vla_expr2, align 8
  %arrayidx4 = getelementptr inbounds i32, ptr %vla3, i64 55
  store i32 23525, ptr %arrayidx4, align 4
  store i32 100, ptr %e, align 4
  %8 = load i32, ptr %e, align 4
  %9 = zext i32 %8 to i64
  %vla5 = alloca %struct.nodeOne, i64 %9, align 16
  store i64 %9, ptr %__vla_expr3, align 8
  store i32 99, ptr getelementptr inbounds ([100 x %struct.nodeOne], ptr @globalThree, i64 0, i64 53), align 16
  call void @populate()
  %10 = load i32, ptr %e, align 4
  call void @populateParam0(ptr noundef %vla5, i32 noundef %10)
  store i32 0, ptr %retval, align 4
  %11 = load ptr, ptr %saved_stack, align 8
  call void @llvm.stackrestore(ptr %11)
  %12 = load i32, ptr %retval, align 4
  ret i32 %12
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn }
attributes #2 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0,1) }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
